#pragma once
#include "FilmQualityInfo.h"
#include "IFpsEnchanterHeader.h"
#include "IFrameEnchanterHeader.h"
#include <vector>

class FilmUpperController {
public:
	FilmUpperController();
	~FilmUpperController();
	int startProcess(std::string filePath, IFrameEnchanterHeader* frameEnchanterHeader, IFpsEnchanterHeader* fpsEnchanterHeader, FilmQualityInfo targetQuality);
	std::vector<IFrameEnchanterHeader*> FrameEnchanterHeaders;
	std::vector<IFpsEnchanterHeader*> FpsEnchanterHeaders;
};