#pragma once
#include "IFrameReader.h"

#define TEST_X_SIZE 10
#define TEST_Y_SIZE 20

class ConstantTestFrameReader: public IFrameReader
{
public:
	VideoFrame* ReadNextFrame() override {
		VideoFrame* frame = new VideoFrame(TEST_X_SIZE, TEST_Y_SIZE);
		int stepSize = INT64_MAX / (TEST_X_SIZE + TEST_Y_SIZE);
		for(int y = 0; y < TEST_Y_SIZE; ++y)
		{
			for(int x = 0; x < TEST_X_SIZE; ++x)
			{
				int blend = stepSize * (x + y);
				frame->Frame[x][y] = new QColor(blend, blend, blend);
			}
		}
		return frame;
	};
	FilmQualityInfo* GetVideoFormatInfo() override {
		auto fq = new FilmQualityInfo();
		fq->FrameRate = 1;
		fq->FrameSizeX = TEST_X_SIZE;
		fq->FrameSizeY = TEST_Y_SIZE;
		return fq;
	};
	bool AreFramesLeft() override {
		if (framesLeftFlag)
		{
			framesLeftFlag = false;
			return true;
		}
		else
			return framesLeftFlag;
	};

private:
	bool framesLeftFlag = true;
};