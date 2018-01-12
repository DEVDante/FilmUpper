#include "BiCubicFrameEnhancer.h"
#include <cmath>

BiCubicFrameEnhancer::BiCubicFrameEnhancer(IFrameReader * inputFrameReader, FilmQualityInfo * targetQualityInfo)
	: FrameEnhancerBase(inputFrameReader, targetQualityInfo)
{
	_nextFrame = new VFHack;
	_framePrefetch = std::thread(PrefetchFrame, inputFrameReader, _nextFrame);

	threads = std::thread::hardware_concurrency();

	threadPoll = new std::thread[threads];
}

VideoFrame * BiCubicFrameEnhancer::ReadNextEnhancedFrame()
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

	for (int t = 0; t < threads; ++t)
	{
		threadPoll[t] = std::thread(CalculateFramePararel, inputFrame, outputFrame, (_targetQualityInfo->Height / threads) * t, (_targetQualityInfo->Height / threads) * (t + 1), _sourceQualityInfo, _targetQualityInfo);
	}

	for (int t = 0; t < threads; ++t)
	{
		threadPoll[t].join();
	}

	delete inputFrame;
	return outputFrame;
}

bool BiCubicFrameEnhancer::AreFramesLeft()
{
	return _framesLeft;
}

BiCubicFrameEnhancer::~BiCubicFrameEnhancer()
{
	delete[] threadPoll;
	delete _nextFrame;
}

void BiCubicFrameEnhancer::PrefetchFrame(IFrameReader* frameReader, VFHack* vf)
{
	vf->Frame = frameReader->ReadNextFrame();
}

void BiCubicFrameEnhancer::CalculateFramePararel(VideoFrame* input, VideoFrame* output, int startRow, int endRow, FilmQualityInfo* sourceQ, FilmQualityInfo* targetQ)
{
	int firstCurveR;
	int secondCurveR;
	int thirdCurveR;
	int fourthCurveR;
	int firstCurveG;
	int secondCurveG;
	int thirdCurveG;
	int fourthCurveG;
	int firstCurveB;
	int secondCurveB;
	int thirdCurveB;
	int fourthCurveB;

	int firstRow;
	int secondRow;
	int thirdRow;
	int fourthRow;

	int finalR;
	int finalG;
	int finalB;

	for (int verticalIndex = startRow; verticalIndex < endRow; ++verticalIndex)
	{
		auto verticalOriginPosition = (double)(verticalIndex * (double)(sourceQ->Height - 1)) / (double)(targetQ->Height - 1);
		auto verticalOriginFloor = (int)verticalOriginPosition;
		auto verticalRatio = verticalOriginPosition - verticalOriginFloor;
		for (int horizontalIndex = 0; horizontalIndex < targetQ->Width; ++horizontalIndex)
		{
			auto horizontalOriginPosition = (double)(horizontalIndex * (double)(sourceQ->Width - 1)) / (double)(targetQ->Width - 1);
			auto horizontalOriginFloor = (int)horizontalOriginPosition;
			auto horizontalRatio = horizontalOriginPosition - horizontalOriginFloor;
			if ((verticalOriginFloor == 0) || !(verticalOriginFloor < (sourceQ->Height - 2)) || (horizontalOriginFloor == 0) || !(horizontalOriginFloor < (sourceQ->Width - 2)))
			{
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3] = input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3];
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1] = input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3 + 1];
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2] = input->Frame[(verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3 + 2];
			}
			else
			{ 
				firstRow = ((verticalOriginFloor - 1) * sourceQ->Width + horizontalOriginFloor) * 3 - 3;
				//First curve
				firstCurveR = calculateSplain(
					input->Frame[firstRow],
					input->Frame[firstRow + 3],
					input->Frame[firstRow + 6],
					input->Frame[firstRow + 9],
					horizontalRatio);
				firstCurveG = calculateSplain(
					input->Frame[firstRow + 1],
					input->Frame[firstRow + 4],
					input->Frame[firstRow + 7],
					input->Frame[firstRow + 10],
					horizontalRatio);
				firstCurveB = calculateSplain(
					input->Frame[firstRow + 2],
					input->Frame[firstRow + 5],
					input->Frame[firstRow + 8],
					input->Frame[firstRow + 11],
					horizontalRatio);

				secondRow = (verticalOriginFloor * sourceQ->Width + horizontalOriginFloor) * 3 - 3;
				//Second curve
				secondCurveR = calculateSplain(
					input->Frame[secondRow],
					input->Frame[secondRow + 3],
					input->Frame[secondRow + 6],
					input->Frame[secondRow + 9],
					horizontalRatio);
				secondCurveG = calculateSplain(
					input->Frame[secondRow + 1],
					input->Frame[secondRow + 4],
					input->Frame[secondRow + 7],
					input->Frame[secondRow + 10],
					horizontalRatio);
				secondCurveB = calculateSplain(
					input->Frame[secondRow + 2],
					input->Frame[secondRow + 5],
					input->Frame[secondRow + 8],
					input->Frame[secondRow + 11],
					horizontalRatio);

				thirdRow = ((verticalOriginFloor + 1) * sourceQ->Width + horizontalOriginFloor) * 3 - 3;
				//Third curve
				thirdCurveR = calculateSplain(
					input->Frame[thirdRow],
					input->Frame[thirdRow + 3],
					input->Frame[thirdRow + 6],
					input->Frame[thirdRow + 9],
					horizontalRatio);
				thirdCurveG = calculateSplain(
					input->Frame[thirdRow + 1],
					input->Frame[thirdRow + 4],
					input->Frame[thirdRow + 7],
					input->Frame[thirdRow + 10],
					horizontalRatio);
				thirdCurveB = calculateSplain(
					input->Frame[thirdRow + 2],
					input->Frame[thirdRow + 5],
					input->Frame[thirdRow + 8],
					input->Frame[thirdRow + 11],
					horizontalRatio);

				fourthRow = ((verticalOriginFloor + 2) * sourceQ->Width + horizontalOriginFloor) * 3 - 3;
				//Fourth curve
				fourthCurveR = calculateSplain(
					input->Frame[fourthRow],
					input->Frame[fourthRow + 3],
					input->Frame[fourthRow + 6],
					input->Frame[fourthRow + 9],
					horizontalRatio);
				fourthCurveG = calculateSplain(
					input->Frame[fourthRow + 1],
					input->Frame[fourthRow + 4],
					input->Frame[fourthRow + 7],
					input->Frame[fourthRow + 10],
					horizontalRatio);
				fourthCurveB = calculateSplain(
					input->Frame[fourthRow + 2],
					input->Frame[fourthRow + 5],
					input->Frame[fourthRow + 8],
					input->Frame[fourthRow + 11],
					horizontalRatio);

				finalR = calculateSplain(firstCurveR, secondCurveR, thirdCurveR, fourthCurveR, verticalRatio);
				finalG = calculateSplain(firstCurveG, secondCurveG, thirdCurveG, fourthCurveG, verticalRatio);
				finalB = calculateSplain(firstCurveB, secondCurveB, thirdCurveB, fourthCurveB, verticalRatio);

				//Final values
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3] = finalR > 255 ? 255 : (finalR < 0 ? 0 : finalR);
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 1] = finalG > 255 ? 255 : (finalG < 0 ? 0 : finalG);
				output->Frame[(verticalIndex * targetQ->Width + horizontalIndex) * 3 + 2] = finalB > 255 ? 255 : (finalB < 0 ? 0 : finalB);
			}			
		}
	}
}

int BiCubicFrameEnhancer::calculateSplain(int a, int b, int c, int d, double ratio)
{
	return 0.5 * (
		((ratio*ratio*(2 - ratio) - ratio) * a) +
		((ratio*ratio*(3 * ratio - 5) + 2) * b) +
		((ratio*ratio*(4 - (3 * ratio)) + ratio) * c) +
		((ratio*ratio*(ratio - 1)) * d)
		);
}