#pragma once
#include "FrameEnchanterBase.h"
#include "FilmQualityInfo.h"

class FpsEnchanterBase {
protected:
	FrameEnchanterBase* _frameEnchanter;
	FilmQualityInfo* _targetQuality;
	FilmQualityInfo* _sourceQuality;
public:
	FpsEnchanterBase(FrameEnchanterBase* frameEnchanter, FilmQualityInfo* targetQuality) {
		_frameEnchanter = frameEnchanter;
		_targetQuality = targetQuality;
		_sourceQuality = _frameEnchanter->GetSourceQuality();
	}

	virtual VideoFrame* ReadNextFrame() { return nullptr; };

	virtual bool AreFramesLeft() { return _frameEnchanter->AreFramesLeft(); };
};