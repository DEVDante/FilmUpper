#include "InterpolationFrameEnchanter.h"
#include <cmath>

InterpolationFrameEnchanter::InterpolationFrameEnchanter(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo)
	: FrameEnchanterBase(inputFrameReader, targetQualityInfo)
{
	_verticalStepBetweenPixels = ((float)_sourceQualityInfo->FrameSizeY)/((float)targetQualityInfo->FrameSizeY);
	_horizontalStepBetweenPixels = ((float)_sourceQualityInfo->FrameSizeX) / ((float)targetQualityInfo->FrameSizeX);
}

VideoFrame * InterpolationFrameEnchanter::ReadNextEnchantedFrame()
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

		for (int verticalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++verticalIndex)
		{
			double nextVerticalIndex = verticalSourceIndex;

			if ((verticalSourceIndex + _verticalStepBetweenPixels) > (_sourceQualityInfo->FrameSizeY - 1))
				nextVerticalIndex = _sourceQualityInfo->FrameSizeY - 1;
			else
				nextVerticalIndex += _verticalStepBetweenPixels;

			double pixelsVerticalRatio = calculateInterpolationRatio(verticalSourceIndex, nextVerticalIndex);

			QColor* leftSample = getInterpolatedColor(
				inputFrame->Frame[floor(horizontalSourceIndex)][ceil(verticalSourceIndex)],
				inputFrame->Frame[floor(horizontalSourceIndex)][ceil(nextVerticalIndex)],
				pixelsVerticalRatio);

			QColor* rightSample = getInterpolatedColor(
				inputFrame->Frame[ceil(nextHorizontalIndex)][ceil(verticalSourceIndex)],
				inputFrame->Frame[ceil(nextHorizontalIndex)][ceil(nextVerticalIndex)],
				pixelsVerticalRatio);

			QColor* outcomeSample = getInterpolatedColor(
				leftSample,
				rightSample,
				pixelsHorizontalRatio);
			outputFrame->Frame[horizontalIndex][verticalIndex] = outcomeSample;

			delete leftSample;
			delete rightSample;

			verticalSourceIndex = nextVerticalIndex;
		}

		horizontalSourceIndex = nextHorizontalIndex;
	}
	delete inputFrame;
	return outputFrame;
}

double InterpolationFrameEnchanter::calculateInterpolationRatio(double leftValue, double rightValue)
{
	return ((((leftValue + rightValue) / 2.0) - floor(leftValue))//Center of actual points
		/ (((floor(leftValue) + ceil(rightValue)) / 2.0) - floor(leftValue))) // Center of point between indexes
		/ 2.0; //If it's nearer next cell then the outcome would be > 1 so we divide it by 2
}

QColor * InterpolationFrameEnchanter::getInterpolatedColor(QColor * leftColor, QColor * rightColor, double blendRatio)
{
	int red = (int)(((double)leftColor->red() * blendRatio) + ((double)rightColor->red() * (1 - blendRatio)));
	int green = (int)(((double)leftColor->green() * blendRatio) + ((double)rightColor->green() * (1 - blendRatio)));
	int blue = (int)(((double)leftColor->blue() * blendRatio) + ((double)rightColor->blue() * (1 - blendRatio)));
	return new QColor(red, green, blue);
}
