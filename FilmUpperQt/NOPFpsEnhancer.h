#pragma once
#include "FpsEnhancerBase.h"
#include "VideoFrame.h"

class NOPFpsEnhancer : public FpsEnhancerBase {
public:
	NOPFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* targetQuality)
		: FpsEnhancerBase(frameEnhancer, targetQuality){
		auto sourceQ = _frameEnhancer->GetSourceQuality();
		_targetQuality->FrameRate->num = sourceQ->FrameRate->num;
		_targetQuality->FrameRate->den = sourceQ->FrameRate->den;
	};

	VideoFrame* ReadNextFrame() override { return _frameEnhancer->ReadNextEnhancedFrame(); };
	bool AreFramesLeft() override { return _frameEnhancer->AreFramesLeft(); };
};