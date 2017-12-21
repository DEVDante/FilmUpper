#include "NNFrameEnhancer.h"
#include <thread>

VideoFrame* NNFrameEnhancer::ReadNextEnhancedFrame()
{
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Width, _targetQualityInfo->Height);
	VideoFrame* inputFrame = _inputFrameStream->ReadNextFrame();
	int threads = 2;

	std::thread *tt = new std::thread[threads];

	for(int t = 0; t < threads; ++t)
	{
		tt[t] = std::thread(calculateFramePararel,inputFrame, outputFrame, (_targetQualityInfo->Height / threads) * t, (_targetQualityInfo->Height / threads) * (t + 1),_sourceQualityInfo, _targetQualityInfo);
	}

	for (int t = 0; t < threads; ++t)
	{
		tt[t].join();
	}
	
	delete inputFrame;
	return outputFrame;
}

void NNFrameEnhancer::calculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ)
{
	double horizontalRatio = (double)sourceQ->Width / (double)targetQ->Width;
	double verticalRatio = (double)sourceQ->Height / (double)targetQ->Height;

	int horizontalOriginPosition;
	int verticalOriginPosition;
	for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
	{
		verticalOriginPosition = verticalRatio * verticalIndex;
		for (int horizontalIndex = 0; horizontalIndex < targetQ->Width; ++horizontalIndex)
		{
			horizontalOriginPosition = horizontalRatio * horizontalIndex;

			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3] = input->Frame[verticalOriginPosition * sourceQ->Width + horizontalOriginPosition * 3];
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 1] = input->Frame[verticalOriginPosition * sourceQ->Width + horizontalOriginPosition * 3 + 1];
			output->Frame[verticalIndex * targetQ->Width + horizontalIndex * 3 + 2] = input->Frame[verticalOriginPosition * sourceQ->Width + horizontalOriginPosition * 3 + 2];
		}
	}
}


bool NNFrameEnhancer::AreFramesLeft()
{
	return _inputFrameStream->AreFramesLeft();
}

NNFrameEnhancer::NNFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo) : FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
}
