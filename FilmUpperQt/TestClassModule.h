#pragma once
#include "FrameEnhancerBase.h"
#include "ConstantTestFrameReader.h"
#include "InterpolationFrameEnhancer.h"
#include "FrameReader.h"
#include "FrameWriter.h"
#include "NNFrameEnhancer.h"
#include <stdexcept>
#include <fstream>
#include <time.h>

class TestClassModule {
public:
	TestClassModule() {};

	void static RunAllTests()
	{
		
	}

	void static RunAllToFileTests()
	{
		//InterpolationFrameEnhTestToFile();
		NNFrameEnhToFileTest();
		ReaderTest();
		//WriterTest();
	}

	void static ReaderTest()
	{
			IFrameReader* frameReader = new FrameReader("D:/Studia/Inzynierka/big_buck_bunny_480p_surround-fix.avi");
			FilmQualityInfo* qualityInfo = new FilmQualityInfo();
			qualityInfo->Width = 1708;
			qualityInfo->Height = 960;
			qualityInfo->FrameRate = new FrameRate(24,1);
			FrameEnhancerBase* enhancer;
			enhancer = new NNFrameEnhancer(frameReader, qualityInfo);
			std::ofstream saveFile;
			saveFile.open("readTestNN.txt");
			int iterations = 10000;
			clock_t begin = clock();
			for (int xd = 0; xd < iterations; ++xd) {
				delete enhancer->ReadNextEnhancedFrame();
			}
			clock_t end = clock();
			double elapsed_secs = (double(end - begin)/iterations) / CLOCKS_PER_SEC;
			saveFile << std::to_string(elapsed_secs) + " sekund dla " + std::to_string(iterations) +" iteracji, \n";
			auto readFrame = enhancer->ReadNextEnhancedFrame();

			for (int y = 0; y < qualityInfo->Height; ++y)
			{
				for (int x = 0; x < qualityInfo->Width; ++x)
				{
					saveFile << std::to_string(readFrame->Frame[y * qualityInfo->Width + x * 3]) + " ";
					saveFile << std::to_string(readFrame->Frame[y * qualityInfo->Width + x * 3 + 1]) + " ";
					saveFile << std::to_string(readFrame->Frame[y * qualityInfo->Width + x * 3 + 2]) + ", ";
				}
				saveFile << "\n";
			}
			saveFile.close();
			delete enhancer;
			delete readFrame;
			delete qualityInfo;
			delete frameReader;
	}

	void static WriterTest()
	{
		IFrameReader* frameReader = new FrameReader("D:/Studia/Inzynierka/big_buck_bunny_480p_surround-fix.avi");
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Width = 1708;
		qualityInfo->Height = 960;
		qualityInfo->FrameRate = new FrameRate(24, 1);
		FrameEnhancerBase* frameEnhancer;
		frameEnhancer = new NNFrameEnhancer(frameReader, qualityInfo);
		FpsEnhancerBase* fpsEnhancer;
		fpsEnhancer = new NOPFpsEnhancer(frameEnhancer, qualityInfo);
		//FrameWriter frameWriter = new FrameWriter()


		delete frameEnhancer;
		delete qualityInfo;
		delete frameReader;
	}

	void static NNFrameEnhToFileTest()
	{
		IFrameReader* frameReader = new ConstantTestFrameReader();
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Height = 30;
		qualityInfo->Width = 60;
		qualityInfo->FrameRate = new FrameRate(1,1);
		FrameEnhancerBase* enhancer;
		enhancer = new NNFrameEnhancer(frameReader, qualityInfo);
		auto readFrame = enhancer->ReadNextEnhancedFrame();

		std::ofstream saveFile;
		saveFile.open("NNFrameEnhTest.txt");
		for (int y = 0; y < qualityInfo->Height; ++y)
		{
			for (int x = 0; x < qualityInfo->Width; ++x)
			{
				saveFile << std::to_string(readFrame->Frame[y * qualityInfo->Width + x * 3]) + " ";
				saveFile << std::to_string(readFrame->Frame[y * qualityInfo->Width + x * 3 + 1]) + " ";
				saveFile << std::to_string(readFrame->Frame[y * qualityInfo->Width + x * 3 + 2]) + ", ";
			}
			saveFile << "\n";
		}
		saveFile.close();
		delete enhancer;
		delete readFrame;
		delete qualityInfo;
		delete frameReader;
	}

	//void static InterpolationFrameEnhTestToFile()
	//{
	//	IFrameReader* frameReader = new ConstantTestFrameReader();
	//	FilmQualityInfo* qualityInfo = new FilmQualityInfo();
	//	qualityInfo->FrameSizeX = 30;
	//	qualityInfo->FrameSizeY = 60;
	//	qualityInfo->FrameRate = 1;
	//	FrameEnhancerBase* enhancer;
	//	enhancer = new InterpolationFrameEnhancer(frameReader, qualityInfo);
	//	auto readFrame = enhancer->ReadNextEnhancedFrame();

	//	std::ofstream saveFile;
	//	saveFile.open("InterpolationFrameEnhTest.txt");

	//	for (int x = 0; x < qualityInfo->FrameSizeX; ++x)
	//	{
	//		for (int y = 0; y < qualityInfo->FrameSizeY; ++y)
	//		{
	//			auto color = readFrame->Frame[x][y];
	//			saveFile << std::to_string(color->red()) + " ";
	//			saveFile << std::to_string(color->green()) + " ";
	//			saveFile << std::to_string(color->blue()) + ", ";
	//		}
	//		saveFile << "\n";
	//	}
	//	saveFile.close();
	//	delete enhancer;
	//	delete readFrame;
	//	delete qualityInfo;
	//	delete frameReader;
	//}

	/*for (int i = 0; i<h2; i++)
	{
		int* t = temp + i*w2;
		y2 = ((i*y_ratio) >> 16);
		int* p = pixels + y2*w1;
		int rat = 0;
		for (int j = 0; j<w2; j++)
		{
			x2 = (rat >> 16);
			*t++ = p[x2];
			rat += x_ratio;
		}
	}*/
};
