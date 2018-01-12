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

int FilmUpperController::startProcess(std::string fileSourcePath, std::string fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality)
{
	FrameReader *frameReader = new FrameReader(fileSourcePath);
	FrameEnhancerBase* frameEnhancer = frameEnhancerHeader->Enhancer(frameReader, targetQuality);
	FpsEnhancerBase* fpsEnhancer = fpsEnhancerHeader->GetFpsEnhancer(frameEnhancer, targetQuality);
	FrameWriter *frameWriter = new FrameWriter(fileTargetPath, "avi", targetQuality);

	while (fpsEnhancer->AreFramesLeft())
	{
		auto fr = fpsEnhancer->ReadNextFrame();
		if (fr == nullptr)
			break;
		frameWriter->WriteFrame(fr);
		//Update gui
	}	

	delete frameWriter;
	delete frameEnhancer;
	delete fpsEnhancer;
	delete targetQuality;
	delete frameReader;

	return 0;
}
