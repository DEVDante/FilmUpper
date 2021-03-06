#include "NNFrameEnhancer.h"

VideoFrame* NNFrameEnhancer::ReadNextEnhancedFrame()
{
	if (!_framesLeft)
		return nullptr;
	_framePrefetch.join();
	VideoFrame* outputFrame = new VideoFrame(_targetQualityInfo->Height, _targetQualityInfo->Width);
	VideoFrame* inputFrame = _nextFrame->Frame;
	if (inputFrame == nullptr)
	{
		_framesLeft = false;
		return nullptr;
	}

	if (_inputFrameStream->AreFramesLeft())
		_framePrefetch = std::thread(PrefetchFrame, _inputFrameStream, _nextFrame);
	else
		_framesLeft = false;

	for(int t = 0; t < _threads; ++t)
	{
		_threadPool[t] = std::thread(CalculateFramePararel, inputFrame, outputFrame, (_targetQualityInfo->Height / _threads) * t, (_targetQualityInfo->Height / _threads) * (t + 1),_sourceQualityInfo, _targetQualityInfo);
	}

	for (int t = 0; t < _threads; ++t)
	{
		_threadPool[t].join();
	}
	
	delete inputFrame;
	return outputFrame;
}

void NNFrameEnhancer::CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ)
{
	double horizontalRatio = (double)sourceQ->Width / (double)targetQ->Width;
	double verticalRatio = (double)sourceQ->Height / (double)targetQ->Height;

	int horizontalOriginPosition;
	int verticalOriginPosition;		
	for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
	{
		verticalOriginPosition = (double)(verticalIndex * (sourceQ->Height-1)) / (double)(targetQ->Height - 1); //verticalRatio * verticalIndex;
		for (int horizontalIndex = 0; horizontalIndex < targetQ->Width; ++horizontalIndex)
		{
			horizontalOriginPosition = (double)(horizontalIndex * (sourceQ->Width - 1)) / (double)(targetQ->Width - 1);//horizontalRatio * horizontalIndex;

			output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3] = input->Frame[(verticalOriginPosition * sourceQ->Width + horizontalOriginPosition) * 3];
			output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1] = input->Frame[(verticalOriginPosition * sourceQ->Width + horizontalOriginPosition) * 3 + 1];
			output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2] = input->Frame[(verticalOriginPosition * sourceQ->Width + horizontalOriginPosition) * 3 + 2];
		}
	}
}

void NNFrameEnhancer::PrefetchFrame(IFrameReader* frameReader, VFHack* vf)
{
	vf->Frame = frameReader->ReadNextFrame();
}


bool NNFrameEnhancer::AreFramesLeft()
{
	return _framesLeft;
}

NNFrameEnhancer::NNFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo) : FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
	_nextFrame = new VFHack;
	_framePrefetch = std::thread(PrefetchFrame, inputFrameReader, _nextFrame);
	_threads = (std::thread::hardware_concurrency() / 4) + 1;
	_threadPool = new std::thread[_threads];
}

NNFrameEnhancer::~NNFrameEnhancer()
{
	delete[] _threadPool;
	delete _nextFrame;
}