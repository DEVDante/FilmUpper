#pragma once
#include "FrameEnhancerBase.h"
#include "ConstantTestFrameReader.h"
#include "InterpolationFrameEnhancer.h"
#include <fstream>

class TestClassModule {
public:
	TestClassModule() {};

	void static RunAllTests()
	{
		InterpolationFrameEnhTest();
	}

	void static InterpolationFrameEnhTest()
	{
		//test case
		IFrameReader* frameReader = new ConstantTestFrameReader();
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->FrameSizeX = 20;
		qualityInfo->FrameSizeY = 40;
		qualityInfo->FrameRate = 1;
		FrameEnhancerBase* enhancer;
		enhancer = new InterpolationFrameEnhancer(frameReader, qualityInfo);
		auto readFrame = enhancer->ReadNextEnchantedFrame();

		std::ofstream saveFile;
		saveFile.open("InterpolationFrameEnhTest.txt");

		for (int x = 0; x < qualityInfo->FrameSizeX; ++x)
		{
			for (int y = 0; y < qualityInfo->FrameSizeY; ++y)
			{
				auto color = readFrame->Frame[x][y];
				saveFile << std::to_string(color->red()) + " ";
				saveFile << std::to_string(color->green()) + " ";
				saveFile << std::to_string(color->blue()) + ", ";
			}
			saveFile << "\n";
		}
		saveFile.close();
		delete enhancer;
		delete readFrame;
		delete qualityInfo;
		delete frameReader;
	}
};
