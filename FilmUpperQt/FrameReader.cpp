#include "FrameReader.h"


FrameReader::FrameReader( std::string filename )
{
	av_register_all();

	if (avformat_open_input(&formatCTX, filename.c_str(), NULL, NULL) != 0)
		throw std::runtime_error("Couldn't open file");

	if (avformat_find_stream_info(formatCTX, NULL)<0)
		throw std::runtime_error("Couldn't read from file");

	videoStream = av_find_best_stream(formatCTX, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0);
	if (videoStream < 0)
		throw std::runtime_error("No video stream found");

	codecCtxOriginal = formatCTX->streams[videoStream]->codec;

	codec = avcodec_find_decoder(codecCtxOriginal->codec_id);
	if (codec == NULL) 
		throw std::runtime_error("Unsupported codec");

	codecCTX = avcodec_alloc_context3(codec);
	if (avcodec_copy_context(codecCTX, codecCtxOriginal) != 0) 
		throw std::runtime_error("Couldn't copy codec context");

	if (avcodec_open2(codecCTX, codec, NULL)<0)
		throw std::runtime_error("Couldn't open codec");

    frame=av_frame_alloc();
    if (frame == NULL)
        throw std::bad_alloc::bad_alloc();
    frameRGB=av_frame_alloc();
    if (frameRGB == NULL)
		throw std::bad_alloc::bad_alloc();
	frameRGB->format = AV_PIX_FMT_RGB24;
	frameRGB->width = codecCTX->width;
	frameRGB->height = codecCTX->height;

	av_frame_get_buffer(frameRGB, 0);

	sws_ctx = sws_getContext(codecCTX->width, codecCTX->height, codecCTX->pix_fmt, codecCTX->width, codecCTX->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    //numBytes=avpicture_get_size(AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height);
    //frameBuffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    //avpicture_fill((AVPicture *)frameRGB, frameBuffer, AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height);

}


FrameReader::~FrameReader()
{
	avcodec_close(codecCTX);
	avcodec_close(codecCtxOriginal);
	avformat_close_input(&formatCTX);
}

bool FrameReader::AreFramesLeft()
{
    return true;
}

VideoFrame* FrameReader::ReadNextFrame()
{
    int frameFinished;
    AVPacket packet;

	while (av_read_frame(formatCTX, &packet) >= 0) {
		if (packet.stream_index == videoStream) {
			avcodec_decode_video2(codecCTX, frame, &frameFinished, &packet);
			if (frameFinished) {
				break;
			}
		}
	}

    sws_scale(sws_ctx, (uint8_t const * const *)frame->data, frame->linesize, 0, codecCTX->height, frameRGB->data, frameRGB->linesize);
    
    VideoFrame *outFrame = new VideoFrame(codecCTX->width, codecCTX->height);

	for (int i = 1; i < codecCTX->height * frameRGB->linesize[0]; i++)
	{
		outFrame->Frame[i] = frameRGB->data[0][i];
	}

	return outFrame;
}

FilmQualityInfo* FrameReader::GetVideoFormatInfo()
{
    FilmQualityInfo *nfo = new FilmQualityInfo();

    nfo->Width = codecCTX->width;
    nfo->Height = codecCTX->height;
	nfo->FrameRate = new FrameRate((codecCTX->framerate.num), (codecCTX->framerate.den));

	return nfo;
}