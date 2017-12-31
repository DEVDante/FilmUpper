#pragma once
#include "FrameEnhancerBase.h"
#include <thread>
#include "VFHack.h"

#define KERNEL_RADIUS 1
#define KERNEL_DIAMETER 3

class MaskFrameEnhancer : public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	MaskFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
		
private:
	void static CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
	void static PrefetchFrame(IFrameReader* frameReader, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFrame;
	bool _framesLeft = true;

	static const int kernel[3][3];
};