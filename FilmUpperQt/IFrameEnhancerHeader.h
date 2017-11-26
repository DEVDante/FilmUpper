#pragma once
#include <string>
#include "FrameEnhancerBase.h"

class IFrameEnhancerHeader
{
public:
	IFrameEnhancerHeader(std::string name, std::string desription)
	{
		Name = name;
		Description = desription;
	}
	std::string Name;
	std::string Description;
	virtual FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) { return new FrameEnhancerBase(inputFrameReader, targetQualityInfo); };
};