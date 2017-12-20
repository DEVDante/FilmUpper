#include "FrameWriter.h"
FrameWriter::FrameWriter( std::string filename, std::string format_name, std::string enc_name, FilmQualityInfo *info)
{
	av_register_all();

	if (avformat_alloc_output_context2(&formatCTX, NULL, format_name.c_str(), filename.c_str()) != 0)
		throw std::runtime_error("Couldn't allocate output format context");

	codec = avcodec_find_encoder_by_name(enc_name.c_str());
	if (codec == NULL) 
		throw std::runtime_error("Couldn't find given encoder");

	codecCTX = avcodec_alloc_context3(codec);
	codecCTX->bit_rate = 400000; //add to FQInfo instead of static
	codecCTX->width = info->FrameSizeX;
	codecCTX->height = info->FrameSizeY;
	codecCTX->time_base = AVRational { info->FrameRate->den , info->FrameRate->num };
	codecCTX->framerate = AVRational { info->FrameRate->num , info->FrameRate->den };
	codecCTX->gop_size = 12; // group of pictures
	codecCTX->max_b_frames = 2; // num of between frames, add check for AV_CODEC_ID_MPEG2VIDEO
	codecCTX->mb_decision = 2; // macroblocks, add check for AV_CODEC_ID_MPEG1VIDEO
	codecCTX->pix_fmt = AV_PIX_FMT_YUV420P; // pixel format

	if (formatCTX->oformat->flags & AVFMT_GLOBALHEADER)
		codecCTX->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	if (avcodec_open2(codecCTX, codec, NULL)<0)
		throw std::runtime_error("Couldn't open codec");

	file->open(filename, std::ios::binary | std::ios::out);
	
	if (!file)
		throw std::runtime_error("Couldn't open file");


    frame=av_frame_alloc();
    if (frame == NULL)
        throw std::bad_alloc::bad_alloc();
    frameRGB=av_frame_alloc();
    if (frameRGB == NULL)
		throw std::bad_alloc::bad_alloc();

    int numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height);
	uint8_t *frameBuffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    avpicture_fill((AVPicture *)frameRGB, frameBuffer, AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height);

	numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, codecCTX->width, codecCTX->height);
	frameBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	avpicture_fill((AVPicture *)frame, frameBuffer, AV_PIX_FMT_YUV420P, codecCTX->width, codecCTX->height);


	if (!(formatCTX->oformat->flags & AVFMT_NOFILE)) 
		if (avio_open(&formatCTX->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0) 
			throw std::runtime_error("Couldn't open file");

	if (avformat_write_header(formatCTX, NULL) < 0)
		throw std::runtime_error("Couldn't write header to file");
}


FrameWriter::~FrameWriter()
{
	file->close();
	avcodec_close(codecCTX);
	av_frame_free(&frame);
	av_frame_free(&frameRGB);
	avformat_close_input(&formatCTX);
}


void FrameWriter::WriteFrame(VideoFrame *frameOG)
{
    struct SwsContext *sws_ctx = NULL;
    int frameFinished;
    AVPacket *packet;

	packet = av_packet_alloc();
	if (!packet)
		throw std::bad_alloc();

    sws_ctx = sws_getContext(codecCTX->width, codecCTX->height, AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height, codecCTX->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);

    for(int y = 0; y < codecCTX->height; y++) 
        for(int x = 0; x < codecCTX->width; x++) 
        {
            int offset = 3 * x + y * frameRGB->linesize[0];

			frameRGB->data[0][offset + 0] = frameOG->Frame[x][y][0];
			frameRGB->data[0][offset + 1] = frameOG->Frame[x][y][1];
			frameRGB->data[0][offset + 2] = frameOG->Frame[x][y][2];
        }

	sws_scale(sws_ctx, (uint8_t const * const *)frameRGB->data, frameRGB->linesize, 0, codecCTX->height, frame->data, frame->linesize);

	int ret = avcodec_send_frame(codecCTX, frame);
	if (ret < 0)
		throw std::runtime_error("Couldn't send frame to encoder");

	while (ret >= 0) {
		ret = avcodec_receive_packet(codecCTX, packet);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			break;
		else if (ret < 0)
			throw std::runtime_error("Error during decoding");
		//fwrite(pkt->data, 1, pkt->size, outfile);
		file->write((char*)packet->data, packet->size);
		av_packet_unref(packet);
	}

	av_packet_free(&packet);
}