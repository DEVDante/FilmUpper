#pragma once
#include "FrameEnhancerBase.h"

class NNFrameEnhancer: public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	NNFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
		
};