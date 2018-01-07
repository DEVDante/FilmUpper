#pragma once
#include "IFrameReader.h"
#include <cstdio>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
}

class FrameWriter
{
private:
	AVOutputFormat *format;
	AVFormatContext *formatCTX;

	AVCodec *codec;
	AVCodecContext *codecCTX;
	AVCodecParameters *codecParam ;

	AVStream *videoStream;
	AVFrame *frameRGB;
	AVFrame *frameOut;

	AVDictionary *dict = NULL;
	AVPacket *packet;
	struct SwsContext *sws_ctx = NULL;

	int gotOutput;
	FilmQualityInfo *info;
	uint64_t nextPTS;


public:
	FrameWriter(std::string, std::string, FilmQualityInfo*);
	~FrameWriter();
	void WriteFrame(VideoFrame*); // override;
};
