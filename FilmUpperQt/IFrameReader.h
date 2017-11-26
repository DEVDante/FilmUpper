#pragma once
#include <string>
#include "VideoFrame.h"
#include "FilmQualityInfo.h"

class IFrameReader
{
public:
	virtual VideoFrame* ReadNextFrame() { return new VideoFrame(1, 1); };
	virtual FilmQualityInfo* GetVideoFormatInfo() { return new FilmQualityInfo(); };
	virtual bool AreFramesLeft() { return true; };
};