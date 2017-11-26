#pragma once
#include "FrameEnhancerBase.h"
#include "ConstantTestFrameReader.h"
#include "InterpolationFrameEnhancer.h"
#include <stdexcept>
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

		//Commented lines for file output

		//std::ofstream saveFile;
		//saveFile.open("InterpolationFrameEnhTest.txt");

		for (int x = 0; x < qualityInfo->FrameSizeX; ++x)
		{
			for (int y = 0; y < qualityInfo->FrameSizeY; ++y)
			{
				auto color = readFrame->Frame[x][y];
				int colorShouldBe = ((x == (qualityInfo->FrameSizeX - 1) ? x-1: x) + (y == (qualityInfo->FrameSizeY - 1) ? y - 1 : y)) * 4;
				if (color->red() != colorShouldBe)
					throw std::logic_error("Value of color should be: " + std::to_string(colorShouldBe) + ", was: " + std::to_string(color->red()));
				//saveFile << std::to_string(color->red()) + " ";
				//saveFile << std::to_string(color->green()) + " ";
				//saveFile << std::to_string(color->blue()) + ", ";
			}
			//saveFile << "\n";
		}
		//saveFile.close();
		delete enhancer;
		delete readFrame;
		delete qualityInfo;
		delete frameReader;
	}
};
