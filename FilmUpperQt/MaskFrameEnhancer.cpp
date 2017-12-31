#include "MaskFrameEnhancer.h"

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

	int threads = 2;

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
		if (verticalIndex < KERNEL_RADIUS || verticalIndex > sourceQ->Height- KERNEL_RADIUS)
			continue;
		for (int horizontalIndex = KERNEL_RADIUS; horizontalIndex < (targetQ->Width- KERNEL_RADIUS); ++horizontalIndex)
		{
			for (int kernelX = 0; kernelX < KERNEL_DIAMETER; kernelX++)
				for (int kernelY = 0; kernelY < KERNEL_DIAMETER; kernelY++)
				{
					sumR = sumR + input->Frame[verticalIndex - 1 * sourceQ->Width + horizontalIndex - 1 * 3] * kernel[kernelX][kernelY];
					sumG = sumR + input->Frame[verticalIndex - 1 * sourceQ->Width + horizontalIndex - 1 * 3 + 1] * kernel[kernelX][kernelY];
					sumB = sumR + input->Frame[verticalIndex - 1 * sourceQ->Width + horizontalIndex - 1 * 3 + 2] * kernel[kernelX][kernelY];
				}
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3] = sumR;
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 1] = sumG;
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 2] = sumB;
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
