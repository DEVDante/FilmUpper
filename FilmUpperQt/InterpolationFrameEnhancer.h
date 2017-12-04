#pragma once
#include "FrameEnhancerBase.h"
#include "FilmQualityInfo.h"
#include "VideoFrame.h"

class InterpolationFrameEnhancer: public FrameEnhancerBase {
public:
	InterpolationFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	VideoFrame * ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
};