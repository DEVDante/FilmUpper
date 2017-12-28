#pragma once
#include "IFrameEnhancerHeader.h"
#include "FilmQualityInfo.h"
#include "BiCubicFrameEnhancer.h"

class BiCubicFrameEnhancerHeader : public IFrameEnhancerHeader {
public:
	BiCubicFrameEnhancerHeader() : IFrameEnhancerHeader("BiCubic Interpolation Frame Resizer", "Enchantes visuals in resized video by interpolating source frame using bi-cubic method. Resulting video might by blury.") {};
	FrameEnhancerBase* Enhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new BiCubicFrameEnhancer(inputFrameReader, targetQualityInfo);
	};
};
