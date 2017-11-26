#pragma once
#include "IFrameEnhancerHeader.h"
#include "InterpolationFrameEnhancer.h"
#include "FilmQualityInfo.h"

class InterpolationFrameEnhancerHeader: public IFrameEnhancerHeader {
public:
	InterpolationFrameEnhancerHeader() : IFrameEnhancerHeader("Interpolation Frame Resizer", "Enchantes visuals in resized video by interpolating source frame. Resulting video might by blury.") {};
	FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new InterpolationFrameEnhancer(inputFrameReader, targetQualityInfo);
	};
};