#pragma once
#include <string>
#include "FrameEnchanterBase.h"

class IFrameEnchanterHeader
{
public:
	std::string Name;
	std::string Description;
	virtual FrameEnchanterBase* Enchanter(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) {};
};