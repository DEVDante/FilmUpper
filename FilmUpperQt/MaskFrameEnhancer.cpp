#include "MaskFrameEnhancer.h"

template<typename T>
T MaskFrameEnhancer::clamp(const T & n, const T & lower, const T & upper)
{
	return std::max(lower, std::min(n, upper));
}


VideoFrame* MaskFrameEnhancer::ReadNextEnhancedFrame()
{
	if (!_framesLeft)
		return nullptr;
	_framePrefetch.join();
	VideoFrame* interpolatedFrame = new VideoFrame(_targetQualityInfo->Height, _targetQualityInfo->Width);
	VideoFrame* inputFrame = _nextFrame->Frame;
	if (inputFrame == nullptr)
	{
		_framesLeft = false;
		return nullptr;
	}

	if (_inputFrameStream->AreFramesLeft())
		_framePrefetch = std::thread(PrefetchFrame, _inputFrameStream, _nextFrame);
	else
		_framesLeft = false;

	

	for(int t = 0; t < _threads; ++t)
	{
		_threadPool[t] = std::thread(CalculateFramePararel, inputFrame, interpolatedFrame, (_targetQualityInfo->Height / _threads) * t, (_targetQualityInfo->Height / _threads) * (t + 1),_sourceQualityInfo, _targetQualityInfo);
	}

	for (int t = 0; t < _threads; ++t)
	{
		_threadPool[t].join();
	}

	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Height, _targetQualityInfo->Width);

	for (int t = 0; t < _threads; ++t)
	{
		_threadPool[t] = std::thread(CalculateKernelPararel, interpolatedFrame, outputFrame, (_targetQualityInfo->Height / _threads) * t, (_targetQualityInfo->Height / _threads) * (t + 1), _targetQualityInfo);
	}

	for (int t = 0; t < _threads; ++t)
	{
		_threadPool[t].join();
	}
	
	delete inputFrame;
	delete interpolatedFrame;
	return outputFrame;
}

void MaskFrameEnhancer::CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ)
{
	int leftSampleR;
	int leftSampleG;
	int leftSampleB;

	int rightSampleR;
	int rightSampleG;
	int rightSampleB;

	for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
	{
		auto verticalOriginPosition = (double)(verticalIndex * (double)(sourceQ->Height - 1)) / (double)(targetQ->Height - 1);
		int verticalOriginCelling = static_cast<int>(verticalOriginPosition + 1);
		verticalOriginCelling = verticalOriginCelling < sourceQ->Height ? verticalOriginCelling : sourceQ->Height - 1;
		int verticalOriginFloor = static_cast<int>(verticalOriginPosition);
		double verticalFmod = 1 - (verticalOriginPosition - (int)verticalOriginPosition);
		for (int horizontalIndex = 0; horizontalIndex < targetQ->Width; ++horizontalIndex)
		{
			auto horizontalOriginPosition = (double)(horizontalIndex * (double)(sourceQ->Width - 1)) / (double)(targetQ->Width - 1);
			int horizontalOriginCelling = static_cast<int>(horizontalOriginPosition + 1);
			int horizontalOriginFloor = static_cast<int>(horizontalOriginPosition);
			horizontalOriginCelling = horizontalOriginCelling < sourceQ->Width ? horizontalOriginCelling : sourceQ->Width - 1;
			double horizontalFmod = 1 - (horizontalOriginPosition - (int)horizontalOriginPosition);

			leftSampleR = VideoFrame::BlendColors(input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3], input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginCelling) * 3], horizontalFmod);
			leftSampleG = VideoFrame::BlendColors(input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3 + 1], input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginCelling) * 3 + 1], horizontalFmod);
			leftSampleB = VideoFrame::BlendColors(input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3 + 2], input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginCelling) * 3 + 2], horizontalFmod);

			rightSampleR = VideoFrame::BlendColors(input->Frame[(verticalOriginCelling * sourceQ->Width + horizontalOriginFloor) * 3], input->Frame[(verticalOriginCelling * sourceQ->Width + horizontalOriginCelling) * 3], horizontalFmod);
			rightSampleG = VideoFrame::BlendColors(input->Frame[(verticalOriginCelling * sourceQ->Width + horizontalOriginFloor) * 3 + 1], input->Frame[(verticalOriginCelling * sourceQ->Width + horizontalOriginCelling) * 3 + 1], horizontalFmod);
			rightSampleB = VideoFrame::BlendColors(input->Frame[(verticalOriginCelling * sourceQ->Width + horizontalOriginFloor) * 3 + 2], input->Frame[(verticalOriginCelling * sourceQ->Width + horizontalOriginCelling) * 3 + 2], horizontalFmod);

			output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3] = VideoFrame::BlendColors(leftSampleR, rightSampleR, verticalFmod);
			output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1] = VideoFrame::BlendColors(leftSampleG, rightSampleG, verticalFmod);
			output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2] = VideoFrame::BlendColors(leftSampleB, rightSampleB, verticalFmod);
		}
	}
}

void MaskFrameEnhancer::CalculateKernelPararel(VideoFrame * input, VideoFrame * output, int startRow, int endRow, FilmQualityInfo * targetQ)
{
	
	for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
	{
		for (int horizontalIndex = 0; horizontalIndex < targetQ->Width; ++horizontalIndex)
		{
			if ((verticalIndex < KERNEL_RADIUS) || !(verticalIndex < (targetQ->Height - KERNEL_RADIUS)) || (horizontalIndex < KERNEL_RADIUS) || !(horizontalIndex < (targetQ->Width - KERNEL_RADIUS)))
			{
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3] = input->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3];
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1] = input->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1];
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2] = input->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2];
			}
			else
			{
				int sumR = 0;
				int sumG = 0;
				int sumB = 0;
				for(int kernelY = 0; kernelY < KERNEL_DIAMETER; ++kernelY)
					for(int kernelX = 0; kernelX < KERNEL_DIAMETER; ++kernelX)
					{
						sumR += input->Frame[((verticalIndex - KERNEL_RADIUS + kernelY) * targetQ->Width + horizontalIndex - KERNEL_RADIUS + kernelX) * 3] * kernel[kernelY][kernelX];
						sumG += input->Frame[((verticalIndex - KERNEL_RADIUS + kernelY) * targetQ->Width + horizontalIndex - KERNEL_RADIUS + kernelX) * 3 + 1] * kernel[kernelY][kernelX];
						sumB += input->Frame[((verticalIndex - KERNEL_RADIUS + kernelY) * targetQ->Width + horizontalIndex - KERNEL_RADIUS + kernelX) * 3 + 2] * kernel[kernelY][kernelX];
					}
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3] = clamp(sumR, 0, 255);
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1] = clamp(sumG, 0, 255);
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2] = clamp(sumB, 0, 255);
			}
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

	_threads = std::thread::hardware_concurrency();

	_threadPool = new std::thread[_threads];
}

MaskFrameEnhancer::~MaskFrameEnhancer()
{
	delete[] _threadPool;
	delete _nextFrame;
}

const int MaskFrameEnhancer::kernel[3][3] = { { 0, -1, 0 },{ -1, 5, -1 },{ 0, -1, 0 } };
