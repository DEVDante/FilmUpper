#pragma once
#include "FpsEnchanterBase.h"
#include "VideoFrame.h"

class InterpolationFpsEnchanter : public FpsEnchanterBase {
public:
	InterpolationFpsEnchanter(FrameEnchanterBase* frameEnchanter, FilmQualityInfo* qualityInfo);
	VideoFrame * ReadNextFrame() override;
private:
	VideoFrame* _currentFrame = nullptr;
	VideoFrame* _nextFrame = nullptr;
	double _currentFrameCooficiency = 0.0f;
	double _nextFrameDelta = 0.0f;
	VideoFrame* InterpolateFrames();
};