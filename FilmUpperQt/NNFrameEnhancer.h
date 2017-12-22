#pragma once
#include "FrameEnhancerBase.h"
#include <thread>

class VFHack
{
public:
	VideoFrame* Frame;
	VFHack() { Frame = nullptr; };
};

class NNFrameEnhancer: public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	NNFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
		
private:
	void static CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
	void static PrefetchFrame(IFrameReader* frameReader, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFrame;
};