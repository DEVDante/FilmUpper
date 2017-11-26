#pragma once
#include <string>
#include "FpsEnhancerBase.h"

class IFpsEnhancerHeader {
public:
	IFpsEnhancerHeader(std::string name, std::string description)
	{
		Name = name;
		Description = description;
	}
	std::string Name;
	std::string Description;
	virtual FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo) { return new FpsEnhancerBase(frameEnhancer, qualityInfo); };
};