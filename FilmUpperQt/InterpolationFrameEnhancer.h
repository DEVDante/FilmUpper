#pragma once
#include "FrameEnhancerBase.h"
#include "FilmQualityInfo.h"
#include "VideoFrame.h"

class InterpolationFrameEnhancer: public FrameEnhancerBase {
public:
	InterpolationFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	VideoFrame * ReadNextEnchantedFrame() override;
	bool AreFramesLeft() override;
private:
	double _verticalStepBetweenPixels;
	double _horizontalStepBetweenPixels;	
};