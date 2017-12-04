#pragma once
#include "IFpsEnhancerHeader.h"
#include "InterlaceFpsEnhancer.h"

class InterlaceFpsEnhancerHeader: public IFpsEnhancerHeader
{
public:
	InterlaceFpsEnhancerHeader() : IFpsEnhancerHeader("Interlace", "Doubles framerate by creating frames by inserting every second row of pixelst from next frame.") {};

	FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo) override { return new InterlaceFpsEnhancer(frameEnhancer, qualityInfo); };
};
