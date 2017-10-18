#pragma once
#include "IFrameReader.h"

class FrameEnchanterBase
{
protected:
	IFrameReader* _inputFrameStream;

public:
	FrameEnchanterBase(IFrameReader* inputFrameReader) {
		_inputFrameStream = inputFrameReader;
	}

	virtual Frame* ReadNextEnchantedFrame() {

	}
};