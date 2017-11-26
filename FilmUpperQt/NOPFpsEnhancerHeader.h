#pragma once
#include "IFpsEnhancerHeader.h"
#include "NOPFpsEnhancer.h"

class NOPFpsEnhancerHeader: public IFpsEnhancerHeader {
public:
	NOPFpsEnhancerHeader() :IFpsEnhancerHeader("No Fps Change", "This Enhancer does not change framerate.") {};
	FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo) override { 
		return new NOPFpsEnhancer(frameEnhancer, qualityInfo);
	};
};