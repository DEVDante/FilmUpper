#pragma once
#include "FpsEnhancerBase.h"
#include "VideoFrame.h"

class InterpolationFpsEnhancer : public FpsEnhancerBase {
public:
	InterpolationFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo);
	VideoFrame * ReadNextFrame() override;
private:
	VideoFrame* _currentFrame = nullptr;
	VideoFrame* _nextFrame = nullptr;
	double _currentFrameCooficiency = 0.0f;
	double _nextFrameDelta = 0.0f;
	VideoFrame* InterpolateFrames() const;
};