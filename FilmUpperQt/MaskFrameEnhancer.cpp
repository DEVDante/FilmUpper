#include "MaskFrameEnhancer.h"

template<typename T>
static T MaskFrameEnhancer::clamp(const T & n, const T & lower, const T & upper)
{
	return std::max(lower, std::min(n, upper));
}


VideoFrame* MaskFrameEnhancer::ReadNextEnhancedFrame()
{
	if (!_framesLeft)
		return nullptr;
	_framePrefetch.join();
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Width, _targetQualityInfo->Height);
	VideoFrame* inputFrame = _nextFrame->Frame;

	if (_inputFrameStream->AreFramesLeft())
		_framePrefetch = std::thread(PrefetchFrame, _inputFrameStream, _nextFrame);
	else
		_framesLeft = false;

	int threads = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1;

	std::thread *tt = new std::thread[threads];

	for(int t = 0; t < threads; ++t)
	{
		tt[t] = std::thread(CalculateFramePararel, inputFrame, outputFrame, (_targetQualityInfo->Height / threads) * t, (_targetQualityInfo->Height / threads) * (t + 1),_sourceQualityInfo, _targetQualityInfo);
	}

	for (int t = 0; t < threads; ++t)
	{
		tt[t].join();
	}
	
	delete inputFrame;
	return outputFrame;
}

void MaskFrameEnhancer::CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ)
{
	int sumR = 0;
	int sumG = 0;
	int sumB = 0;

	for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
	{
		if ((verticalIndex < KERNEL_RADIUS) || (verticalIndex > sourceQ->Height - KERNEL_RADIUS))
			continue;
		for (int horizontalIndex = KERNEL_RADIUS; horizontalIndex < (targetQ->Width - KERNEL_RADIUS); ++horizontalIndex)
		{
			for (int kernelX = (-1); kernelX < KERNEL_RADIUS + 1; kernelX++)
				for (int kernelY = (-1); kernelY < KERNEL_RADIUS + 1; kernelY++)
				{
					sumR = sumR + input->Frame[(verticalIndex + kernelX) * sourceQ->Width + (horizontalIndex + kernelY) * 3] * kernel[kernelX + 1][kernelY + 1];
					sumG = sumR + input->Frame[(verticalIndex + kernelX) * sourceQ->Width + (horizontalIndex + kernelY) * 3 + 1] * kernel[kernelX + 1][kernelY + 1];
					sumB = sumR + input->Frame[(verticalIndex + kernelX) * sourceQ->Width + (horizontalIndex + kernelY) * 3 + 2] * kernel[kernelX + 1][kernelY + 1];
				}
			uint8_t test = output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3] = clamp(sumR, 0, 255);
			uint8_t test2 = output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 1] = clamp(sumG, 0, 255);
			uint8_t test3 = output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 2] = clamp(sumB, 0, 255);
			test3;
		}
	}
}

void MaskFrameEnhancer::PrefetchFrame(IFrameReader* frameReader, VFHack* vf)
{
	vf->Frame = frameReader->ReadNextFrame();
}


bool MaskFrameEnhancer::AreFramesLeft()
{
	return _framesLeft;
}

MaskFrameEnhancer::MaskFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo) : FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
	_nextFrame = new VFHack;
	_framePrefetch = std::thread(PrefetchFrame, inputFrameReader, _nextFrame);
}

const int MaskFrameEnhancer::kernel[3][3] = { { 0, -1, 0 },{ -1, 5, -1 },{ 0, -1, 0 } };
