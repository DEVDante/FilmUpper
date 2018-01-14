#include "MaskFrameEnhancer.h"

template<typename T>
static T MaskFrameEnhancer::clamp(const T & n, const T & lower, const T & upper)
{
	return std::max(lower, std::min(n, upper));
}


VideoFrame* MaskFrameEnhancer::ReadNextEnhancedFrame()
{
	_framePrefetch.join();
	VideoFrame* inputFrame = _nextFrame->Frame;

	if (!_framesLeft || inputFrame == NULL)
		return nullptr;

	VideoFrame* outputFrame = inputFrame->Clone();

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

	int sum = 0;

	for (int cChannel = 0; cChannel < 3; cChannel++)
		for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
		{
			if ((verticalIndex < KERNEL_RADIUS) || (verticalIndex > sourceQ->Height - KERNEL_RADIUS))
				continue;
			for (int horizontalIndex = KERNEL_RADIUS; horizontalIndex < (targetQ->Width - KERNEL_RADIUS); ++horizontalIndex)
			{
				for (int kernelX = -KERNEL_RADIUS; kernelX <= KERNEL_RADIUS; kernelX++)
					for (int kernelY = -KERNEL_RADIUS; kernelY <= KERNEL_RADIUS; kernelY++) {
						auto test = ((verticalIndex + kernelY) * targetQ->Width + (horizontalIndex + kernelX)) * 3;
						sum += input->Frame[((verticalIndex + kernelY) * targetQ->Width + (horizontalIndex + kernelX)) * 3 + cChannel] * kernel[kernelX + KERNEL_RADIUS][kernelY + KERNEL_RADIUS];
					}
						

				output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + cChannel] = clamp(sum, 0, 255);
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

const int MaskFrameEnhancer::kernel[3][3] = { { 0, -1, 0 },{ -1, 9, -1 },{ 0, -1, 0 } };
