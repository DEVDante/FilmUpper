#pragma once
#include "FrameEnhancerBase.h"

class NNFrameEnhancer: public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	NNFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
		
private:
	void static NNFrameEnhancer::calculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
};