#pragma once

#include "NNFrameEnhancer.h"
#include "IFrameEnhancerHeader.h"

class NNFrameEnhancerHeader: public IFrameEnhancerHeader
{
public:
	FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override { return new NNFrameEnhancer(inputFrameReader, targetQualityInfo); };

	NNFrameEnhancerHeader()
		: IFrameEnhancerHeader("Nearest Neighbour", "Algorithm finds the value of pixel by using values of nearest source pixel")
	{
	}
};