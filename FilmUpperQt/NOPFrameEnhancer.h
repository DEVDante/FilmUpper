#pragma once
#include "FrameEnhancerBase.h"
#include "FilmQualityInfo.h"
#include "VideoFrame.h"

class NOPFrameEnhancer : public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnchantedFrame() override { return _inputFrameStream->ReadNextFrame(); };
	bool AreFramesLeft() override { return _inputFrameStream->AreFramesLeft(); };
	NOPFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo)
		: FrameEnhancerBase(inputFrameReader, targetQualityInfo){

	}
};