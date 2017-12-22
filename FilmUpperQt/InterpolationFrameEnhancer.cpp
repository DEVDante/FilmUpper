#include "InterpolationFrameEnhancer.h"
#include <cmath>

InterpolationFrameEnhancer::InterpolationFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo)
	: FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
}

VideoFrame * InterpolationFrameEnhancer::ReadNextEnhancedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Height, _targetQualityInfo->Width);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();
	int leftSampleR;
	int leftSampleG;
	int leftSampleB;

	int rightSampleR;
	int rightSampleG;
	int rightSampleB;

	for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->Height; ++verticalIndex)
	{
		auto verticalOriginPosition = (double)(verticalIndex * (double)(_sourceQualityInfo->Height - 1)) / (double)(_targetQualityInfo->Height - 1);
		int verticalOriginCelling = static_cast<int>(verticalOriginPosition + 1);
		int verticalOriginFloor = static_cast<int>(verticalOriginPosition);
		double verticalFmod = 1 - (verticalOriginPosition - (int)verticalOriginPosition);
		for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->Width; ++horizontalIndex)
		{
			auto horizontalOriginPosition = (double)(horizontalIndex * (double)(_sourceQualityInfo->Width - 1)) / (double)(_targetQualityInfo->Width - 1);
			int horizontalOriginCelling = static_cast<int>(horizontalOriginPosition + 1);
			int horizontalOriginFloor = static_cast<int>(horizontalOriginPosition);
			double horizontalFmod = 1 - (horizontalOriginPosition - (int)horizontalOriginPosition);

			leftSampleR = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginFloor * 3], inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginCelling * 3], horizontalFmod);
			leftSampleG = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 1], inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 1], horizontalFmod);
			leftSampleB = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 2], inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 2], horizontalFmod);

			rightSampleR = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginFloor * 3], inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginCelling * 3], horizontalFmod);
			rightSampleG = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 1], inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 1], horizontalFmod);
			rightSampleB = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 2], inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 2], horizontalFmod);

			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3] = VideoFrame::BlendColors(leftSampleR, rightSampleR, verticalFmod);
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3 + 1] = VideoFrame::BlendColors(leftSampleG, rightSampleG, verticalFmod);
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3 + 2] = VideoFrame::BlendColors(leftSampleB, rightSampleB, verticalFmod);
		}
	}
	delete inputFrame;
	return outputFrame;
}

bool InterpolationFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}
