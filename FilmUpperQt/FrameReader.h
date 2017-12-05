#pragma once
#include "IFrameReader.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <ffmpeg/swscale.h>

class FrameReader : public IFrameReader
{
private:
	AVFormatContext *formatCTX;
	AVCodecContext *codecCtxOriginal;
	AVCodecContext *codecCTX;
	AVCodec *codec;
    AVFrame *frame;
    AVFrame *frameRGB;

	int videoStream;
    int audioStream;
    uint8_t *frameBuffer;
    int numBytes;

public:
	FrameReader( std::string filename );
	~FrameReader();
	bool AreFramesLeft();
	VideoFrame * ReadNextFrame();
	FilmQualityInfo * GetVideoFormatInfo();
};

