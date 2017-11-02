#pragma once
#include "IFrameReader.h"
#include "FilmQualityInfo.h"

class FrameEnchanterBase
{
protected:
	IFrameReader* _inputFrameStream;
	FilmQualityInfo* _targetQualityInfo;
	FilmQualityInfo* _sourceQualityInfo;


public:
	FrameEnchanterBase(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo) {
		_inputFrameStream = inputFrameReader;
		_targetQualityInfo = targetQualityInfo;
		_sourceQualityInfo = _inputFrameStream->GetVideoFormatInfo();
	}

	virtual VideoFrame* ReadNextEnchantedFrame() { return nullptr; };
};