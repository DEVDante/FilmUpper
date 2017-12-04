#pragma once
#include "FpsEnhancerBase.h"
#include "VideoFrame.h"

class NOPFpsEnhancer : public FpsEnhancerBase {
public:
	NOPFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* targetQuality)
		: FpsEnhancerBase(frameEnhancer, targetQuality){
		
	};

	VideoFrame* ReadNextFrame() override { return _frameEnhancer->ReadNextEnhancedFrame(); };
	bool AreFramesLeft() override { return _frameEnhancer->AreFramesLeft(); };
};