#include "FrameWriter.h"
//#include <opencv2\opencv.hpp>

//auto a = av_string_error(ret);
std::string av_string_error(int err)
{
	char buf[256];
	av_strerror(err, buf, sizeof(buf));
	return std::string(buf);
}

FrameWriter::FrameWriter( std::string filename, std::string format_name, FilmQualityInfo *nfo)
{
	int ret;
	info = nfo;
	nextPTS = 0;

	av_register_all();

	format = av_guess_format(format_name.c_str(), NULL, NULL);
	if(!format)
		throw std::runtime_error("Couldn't guess output format");

	formatCTX = avformat_alloc_context();
	formatCTX->oformat = format;
	memcpy(formatCTX->filename, filename.data(), filename.size() < sizeof(formatCTX->filename) ? filename.size() : sizeof(formatCTX->filename));

	codec = avcodec_find_encoder(format->video_codec);
	if (codec == NULL)
		throw std::runtime_error("Couldn't find given encoder");

	videoStream = avformat_new_stream(formatCTX, codec);
	if(!videoStream)
		throw std::runtime_error("Couldn't create new video stream");
	videoStream->time_base.den = info->FrameRate->num;
	videoStream->time_base.num = info->FrameRate->den;

	if (format->flags & AVFMT_GLOBALHEADER)
		videoStream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	codecParam = videoStream->codecpar;
	codecParam->codec_id = format->video_codec;
	codecParam->codec_type = AVMEDIA_TYPE_VIDEO;
	codecParam->width = info->Width;
	codecParam->height = info->Height;
	codecParam->format = codec->pix_fmts[0];
	codecParam->bit_rate = 50000000;
	codecCTX = avcodec_alloc_context3(codec);
	codecCTX->time_base.den = info->FrameRate->den;
	codecCTX->time_base.num = info->FrameRate->num;
	avcodec_parameters_to_context(codecCTX, codecParam);

	//av_dict_set(&dict, "movflags", "faststart", 0);
	//av_dict_set(&dict, "b", "2.5M", 0);

	if (avcodec_open2(codecCTX, codec, &dict) < 0)
		throw std::runtime_error("Couldn't open codec");
	
	if(avio_open(&formatCTX->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0)
		throw std::runtime_error("Couldn't open given file");

	ret = avformat_write_header(formatCTX, NULL);
	if (ret < 0)
		throw std::runtime_error("Couldn't write header to file");

	// DEST PIX FMT
	frameOut = av_frame_alloc();
	if (frameOut == NULL)
		throw std::bad_alloc::bad_alloc();
	frameOut->format = codecCTX->pix_fmt;
	frameOut->height = codecCTX->height;
	frameOut->width = codecCTX->width;
	//int bufferSize = av_image_get_buffer_size(codecCTX->pix_fmt, codecCTX->width, codecCTX->height, 1);
	//av_image_alloc(frameOut->data, frameOut->linesize, codecCTX->width, codecCTX->height, codecCTX->pix_fmt, 32);
	av_frame_make_writable(frameOut);
	av_frame_get_buffer(frameOut, 0);
	//RGB
	frameRGB = av_frame_alloc();
	if (frameRGB == NULL)
		throw std::bad_alloc::bad_alloc();
	frameRGB->format = AV_PIX_FMT_RGB24;
	frameRGB->height = codecCTX->height;
	frameRGB->width = codecCTX->width;
	frameRGB->linesize[0] = info->Width * 3;
	//bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height, 1);
	//av_image_alloc(frameRGB->data, frameRGB->linesize, codecCTX->width, codecCTX->height, AV_PIX_FMT_RGB24, 32);
	av_frame_make_writable(frameRGB);
	av_frame_get_buffer(frameRGB, 0);

	sws_ctx = sws_getContext(codecCTX->width, codecCTX->height, AV_PIX_FMT_RGB24, codecCTX->width, codecCTX->height, codecCTX->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);
}


FrameWriter::~FrameWriter()
{
	//flush encoder
	while (avcodec_send_frame(codecCTX, NULL) != AVERROR_EOF)
	{
		av_init_packet(packet);

		if (avcodec_receive_packet(codecCTX, packet) == 0) 
		{
			//packet->dts = av_rescale_q_rnd(packet->dts, codecCTX->time_base, videoStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			//packet->pts = av_rescale_q_rnd(packet->pts, codecCTX->time_base, videoStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
			//packet->duration = av_rescale_q(1, codecCTX->time_base, videoStream->time_base);
			av_interleaved_write_frame(formatCTX, packet);
		}

		av_packet_unref(packet);
	}

	av_write_trailer(formatCTX);
	avcodec_free_context(&codecCTX);
	avio_close(formatCTX->pb);
	avformat_free_context(formatCTX);
	//av_freep(frameOut->data); 
	//av_freep(frameOut->data);
	av_frame_free(&frameOut);
	av_frame_free(&frameRGB);
}


void FrameWriter::WriteFrame(VideoFrame *frame)
{
	int ret;
	packet = av_packet_alloc();
	if (!packet)
		throw std::bad_alloc();
	av_init_packet(packet);

	/*for (int i = 0; i < frame->GetBufferSize(); i++)
		frameRGB->data[0][i] = frame->Frame[i];*/
	memcpy(frameRGB->data[0], frame->Frame, frame->GetBufferSize());

	//cv::Mat mat(info->Height, info->Width, CV_8UC3, frameRGB->data[0], frameRGB->linesize[0]);
	//cv::Mat mat2(info->Height, info->Width, CV_8UC3, frame->Frame, info->Width*3);
	//cv::imshow("AVFrame RGB", mat);
	//cv::imshow("Our Struct", mat2);

	delete frame;

	sws_scale(sws_ctx, frameRGB->data, frameRGB->linesize, 0, codecCTX->height, frameOut->data, frameOut->linesize);

	frameOut->pts = nextPTS++;
	if (ret = avcodec_send_frame(codecCTX, frameOut) < 0)
		throw std::runtime_error("Couldn't send frame to encoder");

	ret = avcodec_receive_packet(codecCTX, packet);

	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
		return;
	else if (ret < 0)
		throw std::runtime_error("Couldn't recieve packet from encoder");
	//packet->dts = av_rescale_q_rnd(packet->dts, codecCTX->time_base, videoStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
	//packet->pts = av_rescale_q_rnd(packet->pts, codecCTX->time_base, videoStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
	//packet->duration = av_rescale_q(1, codecCTX->time_base, videoStream->time_base);
	if (av_interleaved_write_frame(formatCTX, packet) < 0)
		throw std::runtime_error("Couldn't save packet to file");
	av_packet_unref(packet);

	av_packet_unref(packet);
}