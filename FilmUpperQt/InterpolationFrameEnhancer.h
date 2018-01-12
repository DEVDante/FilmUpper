#pragma once
#include "FrameEnhancerBase.h"
#include "FilmQualityInfo.h"
#include "VideoFrame.h"
#include <thread>
#include "VFHack.h"


class InterpolationFrameEnhancer: public FrameEnhancerBase {
public:
	InterpolationFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	VideoFrame * ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	~InterpolationFrameEnhancer() override;
private:
	void static CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
	void static PrefetchFrame(IFrameReader* frameReader, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFrame;
	bool _framesLeft = true;
	std::thread *threadPoll;
	int threads = 4;
};
