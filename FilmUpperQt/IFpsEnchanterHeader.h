#pragma once
#include <string>
#include "FpsEnchanterBase.h"

class IFpsEnchanterHeader {
public:
	static const std::string Name;
	static const std::string Description;
	virtual FpsEnchanterBase* GetFpsEnchanter(FrameEnchanterBase* frameEnchanter, FilmQualityInfo* qualityInfo);
};