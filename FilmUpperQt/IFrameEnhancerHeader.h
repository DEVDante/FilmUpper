#pragma once
#include <string>
#include "FrameEnhancerBase.h"

class IFrameEnhancerHeader
{
public:
	std::string Name;
	std::string Description;
	virtual FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) { return new FrameEnhancerBase(inputFrameReader, targetQualityInfo); };
};