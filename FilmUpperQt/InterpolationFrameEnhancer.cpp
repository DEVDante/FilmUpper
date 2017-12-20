#include "InterpolationFrameEnhancer.h"
#include <cmath>

InterpolationFrameEnhancer::InterpolationFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo)
	: FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
}

VideoFrame * InterpolationFrameEnhancer::ReadNextEnhancedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->FrameSizeX, _targetQualityInfo->FrameSizeY);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();
	int leftSampleR;
	int leftSampleG;
	int leftSampleB;

	int rightSampleR;
	int rightSampleG;
	int rightSampleB;

	for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++horizontalIndex)
	{
		auto horizontalOriginPosition = static_cast<double>(horizontalIndex * (_sourceQualityInfo->FrameSizeX - 1) ) / (_targetQualityInfo->FrameSizeX - 1);
		int horizontalOriginCelling = ceil(horizontalOriginPosition) > (_sourceQualityInfo->FrameSizeX - 1) ? _sourceQualityInfo->FrameSizeX - 1 : ceil(horizontalOriginPosition);
		for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->FrameSizeY; ++verticalIndex)
		{
			auto verticalOriginPosition = static_cast<double>(verticalIndex * (_sourceQualityInfo->FrameSizeY - 1) ) / (_targetQualityInfo->FrameSizeY - 1);
			int verticalOriginCelling = ceil(verticalOriginPosition) >(_sourceQualityInfo->FrameSizeY - 1) ? _sourceQualityInfo->FrameSizeY - 1 : ceil(verticalOriginPosition);

			auto frame = inputFrame->Frame;
			leftSampleR = VideoFrame::BlendColors(frame[floor(horizontalOriginPosition)][floor(verticalOriginPosition)][0], frame[floor(horizontalOriginPosition)][verticalOriginCelling][0], 1 - fmod(verticalOriginPosition, 1));
			leftSampleG = VideoFrame::BlendColors(frame[floor(horizontalOriginPosition)][floor(verticalOriginPosition)][1], frame[floor(horizontalOriginPosition)][verticalOriginCelling][1], 1 - fmod(verticalOriginPosition, 1));
			leftSampleB = VideoFrame::BlendColors(frame[floor(horizontalOriginPosition)][floor(verticalOriginPosition)][2], frame[floor(horizontalOriginPosition)][verticalOriginCelling][2], 1 - fmod(verticalOriginPosition, 1));
			rightSampleR = VideoFrame::BlendColors(frame[horizontalOriginCelling][floor(verticalOriginPosition)][0], frame[horizontalOriginCelling][verticalOriginCelling][0], 1 - fmod(verticalOriginPosition, 1));
			rightSampleG = VideoFrame::BlendColors(frame[horizontalOriginCelling][floor(verticalOriginPosition)][1], frame[horizontalOriginCelling][verticalOriginCelling][1], 1 - fmod(verticalOriginPosition, 1));
			rightSampleB = VideoFrame::BlendColors(frame[horizontalOriginCelling][floor(verticalOriginPosition)][2], frame[horizontalOriginCelling][verticalOriginCelling][2], 1 - fmod(verticalOriginPosition, 1));

			outputFrame->Frame[horizontalIndex][verticalIndex][0] = VideoFrame::BlendColors(leftSampleR, rightSampleR, 1 - fmod(horizontalOriginPosition, 1));
			outputFrame->Frame[horizontalIndex][verticalIndex][1] = VideoFrame::BlendColors(leftSampleG, rightSampleG, 1 - fmod(horizontalOriginPosition, 1));
			outputFrame->Frame[horizontalIndex][verticalIndex][2] = VideoFrame::BlendColors(leftSampleB, rightSampleB, 1 - fmod(horizontalOriginPosition, 1));
		}
	}
	delete inputFrame;
	return outputFrame;
}

bool InterpolationFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}
