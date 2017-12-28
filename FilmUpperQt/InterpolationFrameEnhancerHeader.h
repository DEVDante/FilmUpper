#pragma once
#include "IFrameEnhancerHeader.h"
#include "InterpolationFrameEnhancer.h"
#include "FilmQualityInfo.h"

class InterpolationFrameEnhancerHeader: public IFrameEnhancerHeader {
public:
	InterpolationFrameEnhancerHeader() : IFrameEnhancerHeader("BiLinearInterpolation Frame Resizer", "Enchantes visuals in resized video by interpolating source frame using BiLinear method. Resulting video might by blury.") {};
	FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new InterpolationFrameEnhancer(inputFrameReader, targetQualityInfo);
	};
};