#pragma once
#include <string>
#include "Frame.h"

class IFrameReader
{
public:
	IFrameReader(std::string inputFilePath) {}
	virtual Frame* ReadNextFrame() {}
	virtual ~IFrameReader() {}
};