#pragma once
#include <string>
#include "VideoFrame.h"
#include "FilmQualityInfo.h"

class IFrameReader
{
public:
	IFrameReader(std::string inputFilePath) {};
	virtual ~IFrameReader() {};
	virtual VideoFrame* ReadNextFrame() {};
	virtual FilmQualityInfo* GetVideoFormatInfo() {};
	virtual bool AreFramesLeft() { return true; };
};