#pragma once
#include "InterpolationFrameEnhancer.h"
#include <thread>
#include <algorithm>
#include "VFHack.h"

#define KERNEL_RADIUS 1
#define KERNEL_DIAMETER 3

class MaskFrameEnhancer : public FrameEnhancerBase
{
public:
	VideoFrame* ReadNextEnhancedFrame() override;
	bool AreFramesLeft() override;
	MaskFrameEnhancer(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	~MaskFrameEnhancer();

	template <typename T>
	static T clamp(const T& n, const T& lower, const T& upper);
		
private:
	void static CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ);
	void static CalculateKernelPararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* targetQ);
	void static PrefetchFrame(IFrameReader* frameReader, VFHack* vf);
	std::thread _framePrefetch;
	VFHack* _nextFrame;
	bool _framesLeft = true;
	int _threads = 8;

	std::thread* _threadPool;

	static const int kernel[3][3];
};