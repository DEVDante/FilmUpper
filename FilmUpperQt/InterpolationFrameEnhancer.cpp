#include "InterpolationFrameEnhancer.h"
#include <cmath>

InterpolationFrameEnhancer::InterpolationFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo)
	: FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
	_verticalStepBetweenPixels = ((double)_sourceQualityInfo->FrameSizeY)/((double)targetQualityInfo->FrameSizeY);
	_horizontalStepBetweenPixels = ((double)_sourceQualityInfo->FrameSizeX) / ((double)targetQualityInfo->FrameSizeX);
}

VideoFrame * InterpolationFrameEnhancer::ReadNextEnchantedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->FrameSizeX, _targetQualityInfo->FrameSizeY);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();

	for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++horizontalIndex)
	{
		auto horizontalOriginPosition = static_cast<double>(horizontalIndex * (_sourceQualityInfo->FrameSizeX) ) / (_targetQualityInfo->FrameSizeX);
		int horizontalOriginCelling = ceil(horizontalOriginPosition) > (_sourceQualityInfo->FrameSizeX - 1) ? _sourceQualityInfo->FrameSizeX - 1 : ceil(horizontalOriginPosition);
		for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->FrameSizeY; ++verticalIndex)
		{
			auto verticalOriginPosition = static_cast<double>(verticalIndex * (_sourceQualityInfo->FrameSizeY) ) / (_targetQualityInfo->FrameSizeY);
			int verticalOriginCelling = ceil(verticalOriginPosition) >(_sourceQualityInfo->FrameSizeY - 1) ? _sourceQualityInfo->FrameSizeY - 1 : ceil(verticalOriginPosition);

			auto frame = inputFrame->Frame;
			QColor* leftSample = VideoFrame::BlendColors(frame[floor(horizontalOriginPosition)][floor(verticalOriginPosition)], frame[floor(horizontalOriginPosition)][verticalOriginCelling], fmod(verticalOriginPosition, 1));
			QColor* rightSample = VideoFrame::BlendColors(frame[horizontalOriginCelling][floor(verticalOriginPosition)], frame[horizontalOriginCelling][verticalOriginCelling], fmod(verticalOriginPosition, 1));

			QColor* finalSample = VideoFrame::BlendColors(leftSample, rightSample, fmod(horizontalOriginPosition, 1));

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

double InterpolationFrameEnhancer::calculateInterpolationRatio(double leftValue, double rightValue)
{
	return ((((leftValue + rightValue) / 2.0) - floor(leftValue))//Center of actual points
		/ (((floor(leftValue) + ceil(rightValue)) / 2.0) - floor(leftValue))) // Center of point between indexes
		/ 2.0; //If it's nearer next cell then the outcome would be > 1 so we divide it by 2
}
