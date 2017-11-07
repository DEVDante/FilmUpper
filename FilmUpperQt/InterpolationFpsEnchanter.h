#pragma once
#include "FpsEnchanterBase.h"

class InterpolationFpsEnchanter : public FpsEnchanterBase {
public:
	InterpolationFpsEnchanter(FrameEnchanterBase* frameEnchanter, FilmQualityInfo* qualityInfo);
	VideoFrame * ReadNextFrame() override;
private:
	VideoFrame* _currentFrame = nullptr;
	VideoFrame* _nextFrame = nullptr;
	double currentFrameCooficiency = 0.0f;
};