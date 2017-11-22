#pragma once
#include <string>
#include "VideoFrame.h"
#include "FilmQualityInfo.h"

class IFrameReader
{
public:
	virtual VideoFrame* ReadNextFrame() {};
	virtual FilmQualityInfo* GetVideoFormatInfo() {};
	virtual bool AreFramesLeft() { return true; };
};