#pragma once
#include "FpsEnhancerBase.h"

class InterlaceFpsEnhancer: public FpsEnhancerBase
{
public:
	VideoFrame* ReadNextFrame() override;

	bool AreFramesLeft() override;

	InterlaceFpsEnhancer(FrameEnhancerBase* frameEnhancer, FilmQualityInfo* targetQuality)
		: FpsEnhancerBase(frameEnhancer, targetQuality)
	{
		_lastFrame = _frameEnhancer->ReadNextEnhancedFrame();;
	}
private:
	bool _wasLastFrameEven = false;
	VideoFrame* _lastFrame = nullptr;
	VideoFrame* InterlaceFrame();
	bool _framesLeft = true;
};
