#pragma once
#include "FilmQualityInfo.h"
#include "IFpsEnhancerHeader.h"
#include "IFrameEnhancerHeader.h"
#include <vector>
#include <QtCore/qobject.h>

class FilmUpperController: public QObject {
	Q_OBJECT
public:
	FilmUpperController();
	~FilmUpperController();
	void startProcess(std::string fileSourcePath, std::string fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality);
	uint64_t getVideoDuration(std::string fileName);
	int getFrameRate(std::string fileName);
	std::vector<IFrameEnhancerHeader*> FrameEnhancerHeaders;
	std::vector<IFpsEnhancerHeader*> FpsEnhancerHeaders;
signals:
	void secondProcessed(long second);
	void processEnded();
	void exceptionInProcess(std::exception* e);
};
