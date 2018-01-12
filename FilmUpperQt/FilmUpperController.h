#pragma once
#include "FilmQualityInfo.h"
#include "IFpsEnhancerHeader.h"
#include "IFrameEnhancerHeader.h"
#include <vector>

class FilmUpperController {
public:
	FilmUpperController();
	~FilmUpperController();
	int startProcess(std::string fileSourcePath, std::string fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality);
	std::vector<IFrameEnhancerHeader*> FrameEnhancerHeaders;
	std::vector<IFpsEnhancerHeader*> FpsEnhancerHeaders;
};