#pragma once
#include "IFrameReader.h"
#include "FilmQualityInfo.h"

class FrameEnhancerBase
{
protected:
	IFrameReader* _inputFrameStream;
	FilmQualityInfo* _targetQualityInfo;
	FilmQualityInfo* _sourceQualityInfo;


public:
	FrameEnhancerBase(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) {
		_inputFrameStream = inputFrameReader;
		_targetQualityInfo = targetQualityInfo;
		_sourceQualityInfo = _inputFrameStream->GetVideoFormatInfo();
	}

	virtual VideoFrame* ReadNextEnchantedFrame() { return nullptr; };
	FilmQualityInfo* GetSourceQuality() { return _sourceQualityInfo; };
	virtual bool AreFramesLeft() { return _inputFrameStream->AreFramesLeft(); };
};