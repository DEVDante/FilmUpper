#include "FilmUpperController.h"
#include "InterpolationFpsEnhancerHeader.h"
#include "InterpolationFrameEnhancerHeader.h"
#include "NOPFpsEnhancerHeader.h"
#include "NOPFrameEnhancerHeader.h"
#include <iostream>
#include "ConstantTestFrameReader.h"

FilmUpperController::FilmUpperController() {
	FrameEnhancerHeaders.push_back(new NOPFrameEnhancerHeader());
	FrameEnhancerHeaders.push_back(new InterpolationFrameEnhancerHeader());

	FpsEnhancerHeaders.push_back(new NOPFpsEnhancerHeader());
	FpsEnhancerHeaders.push_back(new InterpolationFpsEnhancerHeader());

	//test case
	IFrameReader* frameReader = new ConstantTestFrameReader();
	FilmQualityInfo* qualityInfo = new FilmQualityInfo();
	qualityInfo->FrameSizeX = 20;
	qualityInfo->FrameSizeY = 40;
	qualityInfo->FrameRate = 1;
	FrameEnhancerBase* enhancer;
	for (auto i: FrameEnhancerHeaders)
	{
		if(i->Name == "Interpolation Frame Resizer")
		{
			enhancer = i->Enhancer(frameReader, qualityInfo);
		}
	}
	auto readFrame = enhancer->ReadNextEnchantedFrame();
}

FilmUpperController::~FilmUpperController() {
	
}

int FilmUpperController::startProcess(std::string filePath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo targetQuality)
{
	std::cout << "Start Process";
	return 1;
}
