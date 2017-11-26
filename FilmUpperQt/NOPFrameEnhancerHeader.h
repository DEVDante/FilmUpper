#pragma once
#include <IFrameEnhancerHeader.h>
#include <FrameEnhancerBase.h>
#include "FilmQualityInfo.h"
#include "NOPFrameEnhancer.h"

class NOPFrameEnhancerHeader: public IFrameEnhancerHeader
{
public:
	NOPFrameEnhancerHeader() : IFrameEnhancerHeader("No Frame Change", "No change to frame size, so no resampling algorithm. Fastest but no change in quality") {};
	FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override { return new NOPFrameEnhancer(inputFrameReader, targetQualityInfo); };
};
