#pragma once
#include <string>
#include "FpsEnhancerBase.h"

class IFpsEnhancerHeader {
public:
	static const std::string Name;
	static const std::string Description;
	virtual FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo) { return new FpsEnhancerBase(frameEnhancer, qualityInfo); };
};