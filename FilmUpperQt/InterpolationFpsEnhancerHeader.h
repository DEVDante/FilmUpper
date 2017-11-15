#pragma once
#include "IFpsEnhancerHeader.h"
#include "InterpolationFpsEnhancer.h"

class InterpolationFpsEnhancerHeader : public IFpsEnhancerHeader {
public:
	std::string Name = "Interpolation Frame Resizer";
	std::string Description = "Enchantes visuals in resized video by interpolating source frame. Resulting video might by blury.";
	FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new InterpolationFpsEnhancer(inputFrameReader, targetQualityInfo);
	};
};