#pragma once

#include "MaskFrameEnhancer.h"
#include "IFrameEnhancerHeader.h"

class MaskFrameEnhancerHeader: public IFrameEnhancerHeader
{
public:
	FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override { return new MaskFrameEnhancer(inputFrameReader, targetQualityInfo); };

	MaskFrameEnhancerHeader()
		: IFrameEnhancerHeader("Kernel filter", "Processes the frame using the given kernel/mask")
	{
	}
};