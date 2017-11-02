#pragma once
#include "IFrameEnchanterHeader.h"
#include "InterpolationFrameEnchanter.h"
#include "FilmQualityInfo.h"

class InterpolationFrameEnchanterHeader: IFrameEnchanterHeader {
public:
	std::string Name = "Interpolation Frame Resizer";
	std::string Description = "Enchantes visuals in resized video by interpolating source frame. Resulting video might by blury.";
	FrameEnchanterBase* Enchanter(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) override {
		return new InterpolationFrameEnchanter(inputFrameReader, targetQualityInfo);
	};
};