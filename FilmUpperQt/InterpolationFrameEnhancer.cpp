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
		for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->Width; ++horizontalIndex)
		{
			auto horizontalOriginPosition = (double)(horizontalIndex * (double)(_sourceQualityInfo->Width - 1)) / (double)(_targetQualityInfo->Width - 1);
			int horizontalOriginCelling = static_cast<int>(horizontalOriginPosition + 1);
			int horizontalOriginFloor = static_cast<int>(horizontalOriginPosition);

			leftSampleR = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginFloor * 3], inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginCelling * 3], 1 - fmod(horizontalOriginPosition, 1));
			leftSampleG = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 1], inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 1], 1 - fmod(horizontalOriginPosition, 1));
			leftSampleB = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 2], inputFrame->Frame[verticalOriginFloor * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 2], 1 - fmod(horizontalOriginPosition, 1));

			rightSampleR = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginFloor * 3], inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginCelling * 3], 1 - fmod(horizontalOriginPosition, 1));
			rightSampleG = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 1], inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 1], 1 - fmod(horizontalOriginPosition, 1));
			rightSampleB = VideoFrame::BlendColors(inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginFloor * 3 + 2], inputFrame->Frame[verticalOriginCelling * _sourceQualityInfo->Width + horizontalOriginCelling * 3 + 2], 1 - fmod(horizontalOriginPosition, 1));

			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3] = VideoFrame::BlendColors(leftSampleR, rightSampleR, 1 - fmod(verticalOriginPosition, 1));
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3 + 1] = VideoFrame::BlendColors(leftSampleG, rightSampleG, 1 - fmod(verticalOriginPosition, 1));
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3 + 2] = VideoFrame::BlendColors(leftSampleB, rightSampleB, 1 - fmod(verticalOriginPosition, 1));
		}
	}
	delete inputFrame;
	return outputFrame;
}

bool InterpolationFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}
