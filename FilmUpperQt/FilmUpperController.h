#pragma once
#include "FilmQualityInfo.h"
#include "IFpsEnhancerHeader.h"
#include "IFrameEnhancerHeader.h"
#include <vector>
#include <QtCore/qobject.h>
#include "FrameReader.h"
#include "FrameWriter.h"

class Worker: public QObject
{
	Q_OBJECT
public slots:
	void startProcess(std::string* fileSourcePath, std::string* fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality)
{
		FrameReader* frameReader;
		FrameEnhancerBase* frameEnhancer;
		FpsEnhancerBase* fpsEnhancer;
		FrameWriter* frameWriter;

		try
		{
			frameReader = new FrameReader(*fileSourcePath);
			frameEnhancer = frameEnhancerHeader->Enhancer(frameReader, targetQuality);
			fpsEnhancer = fpsEnhancerHeader->GetFpsEnhancer(frameEnhancer, targetQuality);
			frameWriter = new FrameWriter(*fileTargetPath, "avi", targetQuality);
		}
		catch (std::exception* e)
		{
			emit exceptionInProcess(e);
			return;
		}
		try {
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
				if (frames > targetFps)
				{
					frames -= targetFps;
					++seconds;
					emit secondProcessed(seconds);
				}
			}
		}
		catch (std::exception* e)
		{
			emit exceptionInProcess(e);
		}

		try
		{
			delete frameWriter;
			delete frameEnhancer;
			delete fpsEnhancer;
			delete frameReader;
			delete frameEnhancerHeader;
			delete fpsEnhancerHeader;
			delete targetQuality;
			delete fileSourcePath;
			delete fileTargetPath;
		}
		catch (std::exception* e)
		{
			emit exceptionInProcess(e);
		}
}
signals:
	void secondProcessed(long second);
	void processEnded();
	void exceptionInProcess(std::exception* e);
};


class FilmUpperController {
public:
	FilmUpperController();
	~FilmUpperController();
	Worker* startProcess();// std::string fileSourcePath, std::string fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality);
	uint64_t getVideoDuration(std::string fileName);
	int getFrameRate(std::string fileName);
	std::vector<IFrameEnhancerHeader*> FrameEnhancerHeaders;
	std::vector<IFpsEnhancerHeader*> FpsEnhancerHeaders;
};
