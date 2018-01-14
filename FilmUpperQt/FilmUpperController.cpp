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

	double targetFps = targetQuality->FrameRate->getNumericalRate();
	long seconds = 0;
	int frames = 0;

	while (fpsEnhancer->AreFramesLeft())
	{
		auto fr = fpsEnhancer->ReadNextFrame();
		if (fr == nullptr)
			break;
		frameWriter->WriteFrame(fr);

		++frames;
		if(frames > targetFps)
		{
			frames -= targetFps;
			++seconds;
		}

		//Update gui
	}	

	delete frameWriter;
	delete frameEnhancer;
	delete fpsEnhancer;
	delete targetQuality;
	delete frameReader;

	return 0;
}

uint64_t FilmUpperController::getVideoDuration(std::string fileName)
{
	FrameReader *frameReader = new FrameReader(fileName);
	uint64_t temp = frameReader->getVideoDuration();
	delete frameReader;
	return temp;
}

int FilmUpperController::getFrameRate(std::string fileName)
{
	FrameReader *frameReader = new FrameReader(fileName);
	int temp = frameReader->GetVideoFormatInfo()->FrameRate->getNumericalRate();
	delete frameReader;
	return temp;
}
