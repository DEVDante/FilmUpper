#include "InterpolationFrameEnhancer.h"
#include <cmath>

InterpolationFrameEnhancer::InterpolationFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo)
	: FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
	_nextFrame = new VFHack;
	_framePrefetch = std::thread(PrefetchFrame, inputFrameReader, _nextFrame);
}

VideoFrame * InterpolationFrameEnhancer::ReadNextEnhancedFrame()
{
	_framePrefetch.join();
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Width, _targetQualityInfo->Height);
	VideoFrame* inputFrame = _nextFrame->Frame;

	_framePrefetch = std::thread(PrefetchFrame, _inputFrameStream, _nextFrame);

	int threads = 4;

	std::thread *tt = new std::thread[threads];

	for (int t = 0; t < threads; ++t)
	{
		tt[t] = std::thread(CalculateFramePararel, inputFrame, outputFrame, (_targetQualityInfo->Height / threads) * t, (_targetQualityInfo->Height / threads) * (t + 1), _sourceQualityInfo, _targetQualityInfo);
	}

	for (int t = 0; t < threads; ++t)
	{
		tt[t].join();
	}

	delete inputFrame;
	return outputFrame;
}

bool InterpolationFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}

void InterpolationFrameEnhancer::PrefetchFrame(IFrameReader* frameReader, VFHack* vf)
{
	vf->Frame = frameReader->ReadNextFrame();
}

void InterpolationFrameEnhancer::CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ)
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
		int verticalOriginFloor = static_cast<int>(verticalOriginPosition);
		double verticalFmod = 1 - (verticalOriginPosition - (int)verticalOriginPosition);
		for (int horizontalIndex = 0; horizontalIndex < targetQ->Width; ++horizontalIndex)
		{
			auto horizontalOriginPosition = (double)(horizontalIndex * (double)(sourceQ->Width - 1)) / (double)(targetQ->Width - 1);
			int horizontalOriginCelling = static_cast<int>(horizontalOriginPosition + 1);
			int horizontalOriginFloor = static_cast<int>(horizontalOriginPosition);
			double horizontalFmod = 1 - (horizontalOriginPosition - (int)horizontalOriginPosition);

			leftSampleR = VideoFrame::BlendColors(input->Frame[verticalOriginFloor * sourceQ->Width + horizontalOriginFloor * 3], input->Frame[verticalOriginFloor * sourceQ->Width + horizontalOriginCelling * 3], horizontalFmod);
			leftSampleG = VideoFrame::BlendColors(input->Frame[verticalOriginFloor * sourceQ->Width + horizontalOriginFloor * 3 + 1], input->Frame[verticalOriginFloor * sourceQ->Width + horizontalOriginCelling * 3 + 1], horizontalFmod);
			leftSampleB = VideoFrame::BlendColors(input->Frame[verticalOriginFloor * sourceQ->Width + horizontalOriginFloor * 3 + 2], input->Frame[verticalOriginFloor * sourceQ->Width + horizontalOriginCelling * 3 + 2], horizontalFmod);

			rightSampleR = VideoFrame::BlendColors(input->Frame[verticalOriginCelling * sourceQ->Width + horizontalOriginFloor * 3], input->Frame[verticalOriginCelling * sourceQ->Width + horizontalOriginCelling * 3], horizontalFmod);
			rightSampleG = VideoFrame::BlendColors(input->Frame[verticalOriginCelling * sourceQ->Width + horizontalOriginFloor * 3 + 1], input->Frame[verticalOriginCelling * sourceQ->Width + horizontalOriginCelling * 3 + 1], horizontalFmod);
			rightSampleB = VideoFrame::BlendColors(input->Frame[verticalOriginCelling * sourceQ->Width + horizontalOriginFloor * 3 + 2], input->Frame[verticalOriginCelling * sourceQ->Width + horizontalOriginCelling * 3 + 2], horizontalFmod);

			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3] = VideoFrame::BlendColors(leftSampleR, rightSampleR, verticalFmod);
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 1] = VideoFrame::BlendColors(leftSampleG, rightSampleG, verticalFmod);
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 2] = VideoFrame::BlendColors(leftSampleB, rightSampleB, verticalFmod);
		}
	}
}