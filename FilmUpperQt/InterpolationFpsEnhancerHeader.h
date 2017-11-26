#pragma once
#include "IFpsEnhancerHeader.h"
#include "InterpolationFpsEnhancer.h"

class InterpolationFpsEnhancerHeader : public IFpsEnhancerHeader {
public:
	InterpolationFpsEnhancerHeader() : IFpsEnhancerHeader("Interpolation Frame Resizer", "Enchantes visuals in resized video by interpolating source frame. Resulting video might by blury.") {};
	FpsEnhancerBase* GetFpsEnhancer(FrameEnhancerBase* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new InterpolationFpsEnhancer(inputFrameReader, targetQualityInfo);
	};
};