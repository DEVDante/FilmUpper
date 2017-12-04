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

	for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++horizontalIndex)
	{
		auto horizontalOriginPosition = static_cast<double>(horizontalIndex * (_sourceQualityInfo->FrameSizeX - 1) ) / (_targetQualityInfo->FrameSizeX - 1);
		int horizontalOriginCelling = ceil(horizontalOriginPosition) > (_sourceQualityInfo->FrameSizeX - 1) ? _sourceQualityInfo->FrameSizeX - 1 : ceil(horizontalOriginPosition);
		for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->FrameSizeY; ++verticalIndex)
		{
			auto verticalOriginPosition = static_cast<double>(verticalIndex * (_sourceQualityInfo->FrameSizeY - 1) ) / (_targetQualityInfo->FrameSizeY - 1);
			int verticalOriginCelling = ceil(verticalOriginPosition) >(_sourceQualityInfo->FrameSizeY - 1) ? _sourceQualityInfo->FrameSizeY - 1 : ceil(verticalOriginPosition);

			auto frame = inputFrame->Frame;
			QColor* leftSample = VideoFrame::BlendColors(frame[floor(horizontalOriginPosition)][floor(verticalOriginPosition)], frame[floor(horizontalOriginPosition)][verticalOriginCelling], 1 - fmod(verticalOriginPosition, 1));
			QColor* rightSample = VideoFrame::BlendColors(frame[horizontalOriginCelling][floor(verticalOriginPosition)], frame[horizontalOriginCelling][verticalOriginCelling], 1 - fmod(verticalOriginPosition, 1));

			QColor* finalSample = VideoFrame::BlendColors(leftSample, rightSample, 1 - fmod(horizontalOriginPosition, 1));

			if (outputFrame->Frame[horizontalIndex][verticalIndex] != nullptr)
				delete outputFrame->Frame[horizontalIndex][verticalIndex];
			outputFrame->Frame[horizontalIndex][verticalIndex] = finalSample;
			delete leftSample;
			delete rightSample;
		}
	}
	delete inputFrame;
	return outputFrame;
}

bool InterpolationFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}
