#pragma once
#include "FrameEnhancerBase.h"
#include "FilmQualityInfo.h"
#include "VideoFrame.h"
#include <thread>
#include "VFHack.h"


class BiCubicFrameEnhancer : public FrameEnhancerBase {
public:
	BiCubicFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	VideoFrame * ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
private:
	void static CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
	void static PrefetchFrame(IFrameReader* frameReader, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFrame;
	bool _framesLeft = true;
	inline static int calculateSplain(int a, int b, int c, int d, double ratio);
};
