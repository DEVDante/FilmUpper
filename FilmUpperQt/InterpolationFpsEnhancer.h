#pragma once
#include "FpsEnhancerBase.h"
#include "FrameEnhancerBase.h"
#include "VideoFrame.h"
#include "VFHack.h"
#include <thread>

class InterpolationFpsEnhancer : public FpsEnhancerBase {
public:
	InterpolationFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* qualityInfo);
	VideoFrame * ReadNextFrame() override;
	~InterpolationFpsEnhancer() override;
	bool AreFramesLeft() override;
private:
	VideoFrame* _currentFrame = nullptr;
	VideoFrame* _nextFrame = nullptr;
	double _currentFrameCooficiency = 0.0f;
	double _nextFrameDelta = 0.0f;
	VideoFrame* InterpolateFrames() const;
	bool _framesLeft = true;
	void static PrefetchFrame(FrameEnhancerBase* frameEnhancer, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFramePrefetch;
};
