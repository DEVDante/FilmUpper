#pragma once
#include "IFrameReader.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
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
	AVPacket *packet;

	int videoStream;
    int audioStream;
    uint8_t *frameBuffer;
    int numBytes;

	struct SwsContext *sws_ctx;

public:
	FrameReader( std::string filename );
	~FrameReader();
	bool AreFramesLeft() override;
	uint64_t getVideoDuration();
	VideoFrame * ReadNextFrame() override;
	FilmQualityInfo * GetVideoFormatInfo() override;
};

