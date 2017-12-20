#pragma once
#include "IFrameReader.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
}

class FrameReader : public IFrameReader
{
private:
	AVFormatContext *formatCTX = nullptr;
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
	bool AreFramesLeft() override;
	VideoFrame * ReadNextFrame() override;
	FilmQualityInfo * GetVideoFormatInfo() override;
};

