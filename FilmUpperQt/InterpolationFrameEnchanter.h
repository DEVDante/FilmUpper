#pragma once
#include "FrameEnchanterBase.h"
#include "FilmQualityInfo.h"
#include "VideoFrame.h"

class InterpolationFrameEnchanter: public FrameEnchanterBase {
public:
	InterpolationFrameEnchanter(IFrameReader* inputFrameReader, FilmQualityInfo* targetQualityInfo);
	VideoFrame * ReadNextEnchantedFrame() override;
private:
	double _verticalStepBetweenPixels;
	double _horizontalStepBetweenPixels;

	double calculateInterpolationRatio(double leftValue, double rightValue);
	QColor* getInterpolatedColor(QColor* leftColor, QColor* rightColor, double blendRatio);
};