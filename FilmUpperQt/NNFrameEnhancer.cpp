#include "NNFrameEnhancer.h"

VideoFrame* NNFrameEnhancer::ReadNextEnhancedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Width, _targetQualityInfo->Height);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();

	double horizontalRatio = (double)_sourceQualityInfo->Width / (double)_targetQualityInfo->Width;
	double verticalRatio = (double)_sourceQualityInfo->Height / (double)_targetQualityInfo->Height;

	int horizontalOriginPosition;
	int verticalOriginPosition;
	for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->Height; ++verticalIndex)
	{
		verticalOriginPosition = verticalRatio * verticalIndex;
		for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->Width; ++horizontalIndex)
		{
			horizontalOriginPosition = horizontalRatio * horizontalIndex;
			
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3] = inputFrame->Frame[verticalOriginPosition * _sourceQualityInfo->Width + horizontalOriginPosition * 3];
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3 + 1] = inputFrame->Frame[verticalOriginPosition * _sourceQualityInfo->Width + horizontalOriginPosition * 3 + 1];
			outputFrame->Frame[verticalIndex * _targetQualityInfo->Width + horizontalIndex * 3 + 2] = inputFrame->Frame[verticalOriginPosition * _sourceQualityInfo->Width + horizontalOriginPosition * 3 + 2];
		}
	}
	delete inputFrame;
	return outputFrame;
}

bool NNFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}

NNFrameEnhancer::NNFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo) : FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
}
