#pragma once
#include "FilmQualityInfo.h"
#include "IFpsEnhancerHeader.h"
#include "IFrameEnhancerHeader.h"
#include <vector>

class ProcessInformator
{
public:
	std::exception *error = nullptr;
	long secondsProcesed = 0;
	bool processCompleted = false;
	IFrameEnhancerHeader* frameEnh;
	IFpsEnhancerHeader* fpsEnh;
	FilmQualityInfo* qual;
};

class FilmUpperController {
public:
	FilmUpperController();
	~FilmUpperController();
	static void startProcess(std::string fileSourcePath, std::string fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality, ProcessInformator* inf);
	uint64_t getVideoDuration(std::string fileName);
	int getFrameRate(std::string fileName);
	std::vector<IFrameEnhancerHeader*> FrameEnhancerHeaders;
	std::vector<IFpsEnhancerHeader*> FpsEnhancerHeaders;
};