#include "NNFrameEnhancer.h"

VideoFrame* NNFrameEnhancer::ReadNextEnhancedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->FrameSizeX, _targetQualityInfo->FrameSizeY);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();

	int horizontalRatio = (int)((_sourceQualityInfo->FrameSizeX<<16) / _targetQualityInfo->FrameSizeX) + 1;
	int verticalRatio = (int)((_sourceQualityInfo->FrameSizeY<<16) / _targetQualityInfo->FrameSizeY) + 1;

	double horizontalOriginPosition;
	double verticalOriginPosition;

	for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++horizontalIndex)
	{
		horizontalOriginPosition = ((horizontalRatio * horizontalIndex)>>16);
		for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->FrameSizeY; ++verticalIndex)
		{
			verticalOriginPosition = ((verticalRatio * verticalIndex)>>16);
			outputFrame->Frame[horizontalIndex][verticalIndex][0] = inputFrame->Frame[horizontalOriginPosition][verticalOriginPosition][0];
			outputFrame->Frame[horizontalIndex][verticalIndex][1] = inputFrame->Frame[horizontalOriginPosition][verticalOriginPosition][1];
			outputFrame->Frame[horizontalIndex][verticalIndex][2] = inputFrame->Frame[horizontalOriginPosition][verticalOriginPosition][2];			
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
