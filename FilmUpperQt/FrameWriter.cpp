#include "FrameWriter.h"

static int select_channel_layout(const AVCodec *codec)
{
	const uint64_t *p;
	uint64_t best_ch_layout = 0;
	int best_nb_channels = 0;
	if (!codec->channel_layouts)
		return AV_CH_LAYOUT_STEREO;
	p = codec->channel_layouts;
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);
		if (nb_channels > best_nb_channels) {
			best_ch_layout = *p;
			best_nb_channels = nb_channels;
		}
		p++;
	}
	return best_ch_layout;
}

FrameWriter::FrameWriter( std::string filename, std::string format_name, std::string video_enc_name, std::string audio_enc_name, FilmQualityInfo *nfo)
{
	info = nfo;
	av_register_all();

	if (avformat_alloc_output_context2(&formatCTX, NULL, format_name.c_str(), filename.c_str()) != 0)
		throw std::runtime_error("Couldn't allocate output format context");
	format = formatCTX->oformat;

	videoCodec = avcodec_find_encoder_by_name(video_enc_name.c_str());
	if (videoCodec == NULL)
		throw std::runtime_error("Couldn't find given encoder");
	av_format_set_video_codec(formatCTX, videoCodec);
	audioCodec = avcodec_find_encoder_by_name(audio_enc_name.c_str());
	if (audioCodec == NULL)
		throw std::runtime_error("Couldn't find given encoder");
	av_format_set_audio_codec(formatCTX, audioCodec);

	av_dict_set(&dict, "movflags", "faststart", 0);

	if (format->video_codec != AV_CODEC_ID_NONE)
		AddStream(&videoStream, videoCodec);
	if (format->audio_codec != AV_CODEC_ID_NONE)
		AddStream(&audioStream, audioCodec);

	//vframe setup
	if (videoStream.codecCTX)
	{
		if (avcodec_open2(videoStream.codecCTX, videoCodec, NULL)<0)
			throw std::runtime_error("Couldn't open codec");

		videoStream.frame = av_frame_alloc();
		if (videoStream.frame == NULL)
			throw std::runtime_error("Couldn't allocate stream frame");

		videoStream.frame->format = videoStream.codecCTX->pix_fmt;
		videoStream.frame->width = info->Width;
		videoStream.frame->height = info->Height;

		if (av_frame_get_buffer(videoStream.frame, 32) < 0)
			throw std::runtime_error("Couldn't allocate stream frame data");

		frameRGB = av_frame_alloc();
		if (frameRGB == NULL)
			throw std::bad_alloc::bad_alloc();

		frameRGB->format = videoStream.codecCTX->pix_fmt;
		frameRGB->width = info->Width;
		frameRGB->height = info->Height;

		//if (av_frame_get_buffer(frameRGB, 32) < 0)
		//	throw std::runtime_error("Couldn't allocate stream frame data");

		int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, videoStream.codecCTX->width, videoStream.codecCTX->height);
		uint8_t *frameBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

		avpicture_fill((AVPicture *)frameRGB, frameBuffer, AV_PIX_FMT_RGB24, videoStream.codecCTX->width, videoStream.codecCTX->height);

	}
	videoStream.nextPts = 0;

	//aframe setup
	//if (audioStream.codecCTX)
	//{
	//	if (avcodec_open2(audioStream.codecCTX, audioCodec, NULL)<0)
	//		throw std::runtime_error("Couldn't open codec");

	//	audioStream.frame = av_frame_alloc();
	//	if (audioStream.frame == NULL)
	//		throw std::runtime_error("Couldn't allocate stream frame");

	//	audioStream.frame->channels = audioStream.codecCTX->channels;
	//	audioStream.frame->channel_layout = audioStream.codecCTX->channel_layout;
	//	audioStream.frame->sample_rate = audioStream.codecCTX->sample_rate;

	//	if (av_frame_get_buffer(audioStream.frame, 32) < 0)
	//		throw std::runtime_error("Couldn't allocate stream frame data");
	//}
	//audioStream.nextPts = 0;

	if (!(format->flags & AVFMT_NOFILE))
	{
		auto ret = avio_open(&formatCTX->pb, filename.c_str(), AVIO_FLAG_WRITE);
		if (ret < 0)
			throw std::runtime_error("Couldn't open file");
	}

	if(avformat_write_header(formatCTX, &dict) < 0)
		throw std::runtime_error("Couldn't write header to file");

	sws_ctx = sws_getContext(videoStream.codecCTX->width, videoStream.codecCTX->height, AV_PIX_FMT_RGB24, videoStream.codecCTX->width, videoStream.codecCTX->height, videoStream.codecCTX->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);
}


FrameWriter::~FrameWriter()
{
	av_frame_free(&(videoStream.frame));
	av_frame_free(&(audioStream.frame));

	Encode(&videoStream);
	//Encode(&audioStream);
	av_packet_unref(packet);
	av_write_trailer(formatCTX);

	avcodec_free_context(&videoStream.codecCTX);
	avcodec_free_context(&audioStream.codecCTX);
	av_frame_free(&frameRGB);
	sws_freeContext(sws_ctx);

	////if (!(format->flags & AVFMT_NOFILE))
	////{
	////	auto ret = avio_closep(&formatCTX->pb);
	////	if (ret < 0)
	////		;
	////}

	av_packet_free(&packet);
	avformat_close_input(&formatCTX);
}


void FrameWriter::WriteFrame(VideoFrame *frame)
{
    int frameFinished;

	packet = av_packet_alloc();
	if (!packet)
		throw std::bad_alloc();
	av_init_packet(packet);


	for (int i = 0; i < frame->GetBufferSize(); i++)
		frameRGB->data[0][i] = frame->Frame[i];

	//(uint8_t const * const *)
	sws_scale(sws_ctx, frameRGB->data, frameRGB->linesize, 0, videoStream.codecCTX->height, videoStream.frame->data, videoStream.frame->linesize);

	Encode(&videoStream);
	//Encode(&audioStream);

	av_packet_free(&packet);
}

void FrameWriter::Encode(struct OutStream *ostream)
{
	ostream->frame->pts = ostream->nextPts++;
	int ret = avcodec_send_frame(ostream->codecCTX, ostream->frame);
	if (ret < 0)
		throw std::runtime_error("Couldn't send frame to encoder");

	while (ret >= 0) {
		ret = avcodec_receive_packet(ostream->codecCTX, packet);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			break;
		else if (ret < 0)
			throw std::runtime_error("Error during encoding");
	}

	av_packet_rescale_ts(packet, ostream->codecCTX->time_base, ostream->stream->time_base);
	packet->stream_index = ostream->stream->index;
	if(av_interleaved_write_frame(formatCTX, packet))
		throw std::runtime_error("Couldn't write packet to file");
	av_packet_unref(packet);
}

void FrameWriter::AddStream(OutStream *ostream, AVCodec *codec)
{
	ostream->stream = avformat_new_stream(formatCTX, NULL);
	if(!ostream->stream)
		throw std::runtime_error("Couldn't add new stream");
	//codec->type;

	ostream->stream->id = formatCTX->nb_streams - 1;
	ostream->codecCTX = avcodec_alloc_context3(videoCodec);
	if(!ostream->codecCTX)
		throw std::runtime_error("Couldn't allocate codec context");

	//ostream->codecCTX->codec_id =
	ostream->codecCTX->time_base = AVRational{ info->FrameRate->den , info->FrameRate->num };
	ostream->codecCTX->framerate = AVRational{ info->FrameRate->num , info->FrameRate->den };
	if (codec->type == AVMEDIA_TYPE_VIDEO)
	{
		ostream->codecCTX->width = info->Width;
		ostream->codecCTX->height = info->Height;
		ostream->codecCTX->gop_size = 10;
		ostream->codecCTX->max_b_frames = 1;
		ostream->codecCTX->pix_fmt = AV_PIX_FMT_YUV420P;
		ostream->codecCTX->bit_rate = 30000000; //yt recommended for high fps 2k, check for alternative/variable in program?
	} else
		if (codec->type == AVMEDIA_TYPE_AUDIO)
		{
			ostream->codecCTX->bit_rate = 512000; //yt recommended for 5.1 audio - see upper comment
			ostream->codecCTX->sample_fmt = AV_SAMPLE_FMT_U8;
			ostream->codecCTX->channel_layout = select_channel_layout(audioCodec);
			ostream->codecCTX->channels = av_get_channel_layout_nb_channels(ostream->codecCTX->channel_layout);
			ostream->codecCTX->sample_rate = info->SampleRate;
		}

	//av_opt_set(VideoSt.Ctx->priv_data, "cq", TCHAR_TO_ANSI(*H264Crf), 0);
	//av_opt_set(VideoSt.Ctx->priv_data, "gpu", TCHAR_TO_ANSI(*DeviceNum), 0); --see if needed;

	if(formatCTX->oformat->flags & AVFMT_GLOBALHEADER)
		ostream->codecCTX->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

