#include "NNFrameEnhancer.h"

VideoFrame* NNFrameEnhancer::ReadNextEnhancedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->FrameSizeX, _targetQualityInfo->FrameSizeY);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();

	for (int horizontalIndex = 0; horizontalIndex < _targetQualityInfo->FrameSizeX; ++horizontalIndex)
	{
		auto horizontalOriginPosition = static_cast<double>(horizontalIndex * (_sourceQualityInfo->FrameSizeX - 1)) / (_targetQualityInfo->FrameSizeX - 1);
		int horizontalOriginCelling = ceil(horizontalOriginPosition) >(_sourceQualityInfo->FrameSizeX - 1) ? _sourceQualityInfo->FrameSizeX - 1 : ceil(horizontalOriginPosition);
		for (int verticalIndex = 0; verticalIndex < _targetQualityInfo->FrameSizeY; ++verticalIndex)
		{
			auto verticalOriginPosition = static_cast<double>(verticalIndex * (_sourceQualityInfo->FrameSizeY - 1)) / (_targetQualityInfo->FrameSizeY - 1);
			int verticalOriginCelling = ceil(verticalOriginPosition) >(_sourceQualityInfo->FrameSizeY - 1) ? _sourceQualityInfo->FrameSizeY - 1 : ceil(verticalOriginPosition);

			auto frame = inputFrame->Frame;

			QColor* chosenColor;

			if(fmod(verticalOriginPosition, 1) <= 0.5)
			{
				if(fmod(horizontalOriginPosition, 1) <= 0.5)
				{
					chosenColor = frame[floor(horizontalOriginPosition)][floor(verticalOriginPosition)];
				}
				else
				{
					chosenColor = frame[horizontalOriginCelling][floor(verticalOriginPosition)];
				}
			}
			else
			{
				if (fmod(horizontalOriginPosition, 1) <= 0.5)
				{
					chosenColor = frame[floor(horizontalOriginPosition)][verticalOriginCelling];
				}
				else
				{
					chosenColor = frame[horizontalOriginCelling][verticalOriginCelling];
				}
			}
			QColor* finalSample = new QColor(chosenColor->rgb());

			if (outputFrame->Frame[horizontalIndex][verticalIndex] != nullptr)
				delete outputFrame->Frame[horizontalIndex][verticalIndex];
			outputFrame->Frame[horizontalIndex][verticalIndex] = finalSample;
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
