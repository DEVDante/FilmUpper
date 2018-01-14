#pragma once
#include "FrameEnhancerBase.h"
#include <thread>
#include "VFHack.h"

class NNFrameEnhancer: public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	NNFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	~NNFrameEnhancer() override;
		
private:
	void static CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
	void static PrefetchFrame(IFrameReader* frameReader, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFrame;
	bool _framesLeft = true;
	std::thread* _threadPool;
	int _threads = 4;
};