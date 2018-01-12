#pragma once
#include "FpsEnhancerBase.h"
#include "VFHack.h"
#include <thread>

class InterlaceFpsEnhancer: public FpsEnhancerBase
{
public:
	VideoFrame* ReadNextFrame() override;

	bool AreFramesLeft() override;

	InterlaceFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* targetQuality);
	~InterlaceFpsEnhancer();

private:
	bool _wasLastFrameOdd = false;
	VideoFrame* _lastFrame = nullptr;
	VideoFrame* InterlaceFrame();
	bool _framesLeft = true;
	void static PrefetchFrame(FrameEnhancerBase* frameEnhancer, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFramePrefetch;
};
