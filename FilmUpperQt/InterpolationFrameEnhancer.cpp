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
	double horizontalSourceIndex = 0.0f;
	for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++horizontalIndex)
	{
		double verticalSourceIndex = 0.0f;

		double nextHorizontalIndex = horizontalSourceIndex;
		if ((horizontalSourceIndex + _horizontalStepBetweenPixels) >(_sourceQualityInfo->FrameSizeX - 1))
			nextHorizontalIndex = _sourceQualityInfo->FrameSizeX - 1;
		else
			nextHorizontalIndex += _horizontalStepBetweenPixels;

		double pixelsHorizontalRatio = calculateInterpolationRatio(horizontalSourceIndex, nextHorizontalIndex);

		for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->FrameSizeX; ++verticalIndex)
		{
			double nextVerticalIndex = verticalSourceIndex;

			if ((verticalSourceIndex + _verticalStepBetweenPixels) > (_sourceQualityInfo->FrameSizeY - 1))
				nextVerticalIndex = _sourceQualityInfo->FrameSizeY - 1;
			else
				nextVerticalIndex += _verticalStepBetweenPixels;

			double pixelsVerticalRatio = calculateInterpolationRatio(verticalSourceIndex, nextVerticalIndex);

			auto inFrame = inputFrame->Frame;

			QColor* leftSample = VideoFrame::BlendColors(inFrame[floor(horizontalSourceIndex)][ceil(verticalSourceIndex)], inFrame[floor(horizontalSourceIndex)][ceil(nextVerticalIndex)], pixelsVerticalRatio);
			QColor* rightSample = VideoFrame::BlendColors(inFrame[ceil(nextHorizontalIndex)][ceil(verticalSourceIndex)], inFrame[ceil(nextHorizontalIndex)][ceil(nextVerticalIndex)], pixelsVerticalRatio);
			QColor* outcomeSample = VideoFrame::BlendColors(leftSample, rightSample, pixelsHorizontalRatio);
			
			delete leftSample;
			delete rightSample;

			outputFrame->Frame[horizontalIndex][verticalIndex] = outcomeSample;
			verticalSourceIndex = nextVerticalIndex;
		}

		horizontalSourceIndex = nextHorizontalIndex;
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
