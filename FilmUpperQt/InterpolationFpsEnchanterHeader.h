#pragma once
#include "IFpsEnchanterHeader.h"
#include "InterpolationFpsEnchanter.h"
#include "FilmQualityInfo.h"

class InterpolationFrameEnchanterHeader : IFpsEnchanterHeader {
public:
	std::string Name = "Interpolation Frame Resizer";
	std::string Description = "Enchantes visuals in resized video by interpolating source frame. Resulting video might by blury.";
	FpsEnchanterBase* GetFpsEnchanter(FrameEnchanterBase* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new InterpolationFpsEnchanter(inputFrameReader, targetQualityInfo);
	};
};