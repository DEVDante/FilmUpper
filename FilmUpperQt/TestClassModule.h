#pragma once
#include "FrameEnhancerBase.h"
#include "ConstantTestFrameReader.h"
#include "InterpolationFrameEnhancer.h"
#include "NNFrameEnhancer.h"
#include <stdexcept>
#include <fstream>

class TestClassModule {
public:
	TestClassModule() {};

	void static RunAllTests()
	{
		
	}

	void static RunAllToFileTests()
	{
		InterpolationFrameEnhTestToFile();
		NNFrameEnhToFileTest();
	}


	void static NNFrameEnhToFileTest()
	{
		IFrameReader* frameReader = new ConstantTestFrameReader();
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->FrameSizeX = 20;
		qualityInfo->FrameSizeY = 40;
		qualityInfo->FrameRate = 1;
		FrameEnhancerBase* enhancer;
		enhancer = new NNFrameEnhancer(frameReader, qualityInfo);
		auto readFrame = enhancer->ReadNextEnhancedFrame();

		std::ofstream saveFile;
		saveFile.open("NNFrameEnhTest.txt");
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

	void static InterpolationFrameEnhTestToFile()
	{
		IFrameReader* frameReader = new ConstantTestFrameReader();
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->FrameSizeX = 30;
		qualityInfo->FrameSizeY = 60;
		qualityInfo->FrameRate = 1;
		FrameEnhancerBase* enhancer;
		enhancer = new InterpolationFrameEnhancer(frameReader, qualityInfo);
		auto readFrame = enhancer->ReadNextEnhancedFrame();

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
