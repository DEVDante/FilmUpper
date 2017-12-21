#pragma once
#include "IFrameReader.h"
#include <fstream>

#define TEST_HEIGHT 10
#define TEST_WIDTH 20

class ConstantTestFrameReader: public IFrameReader
{
public:
	VideoFrame* ReadNextFrame() override {
		VideoFrame* frame = new VideoFrame(TEST_HEIGHT, TEST_WIDTH);
		int stepSize = 255 / (TEST_HEIGHT + TEST_WIDTH);
		std::ofstream saveFile;
		saveFile.open("InterpolationFrameEnhTestIn.txt");
		for(int y = 0; y < TEST_HEIGHT; ++y)
		{
			for(int x = 0; x < TEST_WIDTH; ++x)
			{
				int blend = stepSize * (x + y);
				frame->Frame[y * TEST_WIDTH + x * 3] = blend;
				frame->Frame[y * TEST_WIDTH + x * 3 + 1] = blend;
				frame->Frame[y * TEST_WIDTH + x * 3 + 2] = blend;
				saveFile << std::to_string(blend) + " in " + std::to_string(y * TEST_WIDTH + x * 3) + ", ";
				saveFile << std::to_string(blend) + " in " + std::to_string(y * TEST_WIDTH + x * 3 + 1) + ", ";
				saveFile << std::to_string(blend) + " in " + std::to_string(y * TEST_WIDTH + x * 3 + 2) + ", ";
			}
			saveFile << "\n";
		}
		saveFile.close();
		return frame;
	};
	FilmQualityInfo* GetVideoFormatInfo() override {
		auto fq = new FilmQualityInfo();
		fq->FrameRate = new FrameRate(1,1);
		fq->Height = TEST_HEIGHT;
		fq->Width = TEST_WIDTH;
		return fq;
	};
	bool AreFramesLeft() override {
		if (framesLeftFlag)
		{
			framesLeftFlag = false;
			return true;
		}
		return framesLeftFlag;
	};

private:
	bool framesLeftFlag = true;
};