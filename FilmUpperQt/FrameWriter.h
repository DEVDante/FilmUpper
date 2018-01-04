#pragma once
#include "IFrameReader.h"
#include <cstdio>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
}

class FrameWriter
{
private:
	AVFormatContext *formatCTX;
	AVOutputFormat *format;
	AVCodec *videoCodec;
	AVCodec *audioCodec;
	AVFrame *frameRGB;
	//AVDictionary *dict = NULL;
	AVPacket *packet;
	struct SwsContext *sws_ctx = NULL;

	OutStream videoStream = { 0 };
	OutStream audioStream = { 0 };
	int gotOutput;
	FilmQualityInfo *info;

	void AddStream(struct OutStream*, AVCodec*);
	void Encode(struct OutStream*);

public:
	FrameWriter(std::string, std::string, std::string, std::string, FilmQualityInfo*);
	~FrameWriter();
	void WriteFrame(VideoFrame*); // override;
};

struct OutStream {
	AVStream *stream;
	AVCodecContext *codecCTX;
	int64_t nextPts;
	AVFrame *frame;
};
