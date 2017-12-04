#include "FilmUpperController.h"
#include "InterpolationFpsEnhancerHeader.h"
#include "InterpolationFrameEnhancerHeader.h"
#include "NOPFpsEnhancerHeader.h"
#include "NOPFrameEnhancerHeader.h"
#include "NNFrameEnhancerHeader.h"
#include <iostream>

#include "TestClassModule.h"
#include "InterlaceFpsEnhancerHeader.h"

FilmUpperController::FilmUpperController() {
	FrameEnhancerHeaders.push_back(new NOPFrameEnhancerHeader());
	FrameEnhancerHeaders.push_back(new InterpolationFrameEnhancerHeader());
	FrameEnhancerHeaders.push_back(new NNFrameEnhancerHeader());

	FpsEnhancerHeaders.push_back(new NOPFpsEnhancerHeader());
	FpsEnhancerHeaders.push_back(new InterpolationFpsEnhancerHeader());
	FpsEnhancerHeaders.push_back(new InterlaceFpsEnhancerHeader());

	TestClassModule::RunAllToFileTests();
}

FilmUpperController::~FilmUpperController() {
	
}

int FilmUpperController::startProcess(std::string filePath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality)
{
	std::cout << "Start Process";
	return 1;
}
