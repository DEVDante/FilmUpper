#pragma once
#include "FrameEnhancerBase.h"
#include "FilmQualityInfo.h"

class FpsEnhancerBase {
protected:
	FrameEnhancerBase* _frameEnhancer;
	FilmQualityInfo* _targetQuality;
	FilmQualityInfo* _sourceQuality;
public:
	FpsEnhancerBase(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* targetQuality) {
		_frameEnhancer = frameEnhancer;
		_targetQuality = targetQuality;
		_sourceQuality = _frameEnhancer->GetSourceQuality();
	}

	virtual VideoFrame* ReadNextFrame() { return nullptr; };

	virtual bool AreFramesLeft() { return _frameEnhancer->AreFramesLeft(); };
};