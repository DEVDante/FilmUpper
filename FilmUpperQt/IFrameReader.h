#pragma once
#include <string>
#include "VideoFrame.h"

class IFrameReader
{
public:
	IFrameReader(std::string inputFilePath) {}
	virtual VideoFrame* ReadNextFrame() {}
	virtual ~IFrameReader() {}
};