#pragma once
#include <string>
#include "FrameEnchanterBase.h"

__interface IFrameEnchanterHeader
{
public:
	std::string Name();
	std::string Description();
	FrameEnchanterBase* Enchanter(IFrameReader* inputFrameReader);
};