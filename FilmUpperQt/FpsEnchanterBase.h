#pragma once
#include "FrameEnchanterBase.h"

class FpsEnchanterBase {
protected:
	FrameEnchanterBase* _frameEnchanter;
public:
	FpsEnchanterBase(FrameEnchanterBase* frameEnchanter) {
		_frameEnchanter = frameEnchanter;
	}

	virtual VideoFrame* ReadNextFrame(){}
};