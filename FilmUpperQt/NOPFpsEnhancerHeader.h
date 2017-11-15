#pragma once
#include "IFpsEnhancerHeader.h"
#include "NOPFpsEnhancer.h"

class NOPFpsEnhancerHeader: public IFpsEnhancerHeader {
public:
	std::string Name = "No Fps Change";
	std::string Description = "This Enhancer";
	FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo) override { 
		return new NOPFpsEnhancer(frameEnhancer, qualityInfo);
	};
};