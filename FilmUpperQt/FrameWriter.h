#pragma once
#include "IFrameReader.h"
#include <fstream>
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
	AVCodecContext *codecCTX;
	AVCodec *codec;
	AVFrame *frame;
	AVFrame *frameRGB;
	std::ofstream *file;

	int videoStream;
	int audioStream;

public:
	FrameWriter(std::string, std::string, std::string, FilmQualityInfo*);
	~FrameWriter();
	void WriteFrame(VideoFrame*); // override;
};
