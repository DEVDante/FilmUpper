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
#include "BiCubicFrameEnhancer.h"
#include "InterlaceFpsEnhancer.h"
#include "MaskFrameEnhancer.h"

class TestClassModule {
public:
	TestClassModule() {};

	void static RunAllTests()
	{
		
	}

	void static RunAllToFileTests()
	{
		//InterpolationFrameEnhTestToFile();
		//NNFrameEnhToFileTest();
		//NNSpeedTest();
		//InterpolationSpeedTest();
		//WriterTest();
		//BiCubicSpeedTest();
		//NOPToFileTest();
		WriterTest();
		//BiCubicSpeedTest();
	}

	void static NNSpeedTest()
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
			int iterations = 1000;
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

	void static BiCubicSpeedTest()
	{
		IFrameReader* frameReader = new FrameReader("D:/Studia/Inzynierka/big_buck_bunny_480p_surround-fix.avi");
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Width = 1708;
		qualityInfo->Height = 960;
		qualityInfo->FrameRate = new FrameRate(24, 1);
		FrameEnhancerBase* enhancer;
		enhancer = new BiCubicFrameEnhancer(frameReader, qualityInfo);
		std::ofstream saveFile;
		saveFile.open("readTestBiCubic.txt");
		int iterations = 1000;
		clock_t begin = clock();
		for (int xd = 0; xd < iterations; ++xd) {
			delete enhancer->ReadNextEnhancedFrame();
		}
		clock_t end = clock();
		double elapsed_secs = (double(end - begin) / iterations) / CLOCKS_PER_SEC;
		saveFile << std::to_string(elapsed_secs) + " sekund dla " + std::to_string(iterations) + " iteracji, \n";
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

	void static InterpolationSpeedTest()
	{
		IFrameReader* frameReader = new FrameReader("sample.mp4");
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Width = 1708 / 2;
		qualityInfo->Height = 960 / 2;
		qualityInfo->FrameRate = new FrameRate(24, 1);
		FrameEnhancerBase* enhancer;
		enhancer = new InterpolationFrameEnhancer(frameReader, qualityInfo);
		std::ofstream saveFile;
		saveFile.open("readTestInterpolation.txt");
		//int iterations = 1000;
		//clock_t begin = clock();
		//for (int xd = 0; xd < iterations; ++xd) {
		//	delete enhancer->ReadNextEnhancedFrame();
		//}
		//clock_t end = clock();
		//double elapsed_secs = (double(end - begin) / iterations) / CLOCKS_PER_SEC;
		//saveFile << std::to_string(elapsed_secs) + " sekund dla " + std::to_string(iterations) + " iteracji, \n";
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
		FrameEnhancerBase* frameEnhancer;
		FrameReader *frameReader = new FrameReader("sample.mp4");
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Width = 3840;
		qualityInfo->Height = 2160;
		qualityInfo->FrameRate = new FrameRate(25, 1);		
		frameEnhancer = new InterpolationFrameEnhancer(frameReader, qualityInfo);
		FpsEnhancerBase* fpsEnhancer = new NOPFpsEnhancer(frameEnhancer, qualityInfo);
		FrameWriter *frameWriter = new FrameWriter("out.avi", "avi", qualityInfo);

		while (frameReader->AreFramesLeft())
		{
			//frameWriter->WriteFrame(frameEnhancer->ReadNextEnhancedFrame());
			auto fr = fpsEnhancer->ReadNextFrame();
			if (fr == nullptr)
				break;
			frameWriter->WriteFrame(fr);
		}

		//for (int i = 0; i < 200; i++)
		//	frameWriter->WriteFrame(fpsEnhancer->ReadNextFrame());


		delete frameWriter;
		delete frameEnhancer;
		delete fpsEnhancer;
		delete qualityInfo;
		delete frameReader;
	}

	void static NOPToFileTest()
	{
		IFrameReader* frameReader = new FrameReader("sample.mp4");
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Height = 960 / 2;
		qualityInfo->Width = 1708 / 2;
		qualityInfo->FrameRate = new FrameRate(24, 1);
		FrameEnhancerBase* enhancer;
		enhancer = new NOPFrameEnhancer(frameReader, qualityInfo);
		auto readFrame = enhancer->ReadNextEnhancedFrame();

		std::ofstream saveFile;
		saveFile.open("NOPFrameEnhTest.txt");
		for (int y = 0; y < qualityInfo->Height; ++y)
		{
			for (int x = 0; x < qualityInfo->Width; ++x)
			{
				saveFile << std::to_string(readFrame->Frame[(y * qualityInfo->Width + x) * 3]) + " ";
				saveFile << std::to_string(readFrame->Frame[(y * qualityInfo->Width + x) * 3 + 1]) + " ";
				saveFile << std::to_string(readFrame->Frame[(y * qualityInfo->Width + x) * 3 + 2]) + ", ";
			}
			saveFile << "\n";
		}
		saveFile.close();
		delete enhancer;
		delete readFrame;
		delete qualityInfo;
		delete frameReader;
	}

	void static NNFrameEnhToFileTest()
	{
		IFrameReader* frameReader = new ConstantTestFrameReader();
		FilmQualityInfo* qualityInfo = new FilmQualityInfo();
		qualityInfo->Height = 100;
		qualityInfo->Width = 200;
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
				saveFile << std::to_string(readFrame->Frame[(y * qualityInfo->Width + x) * 3]) + " ";
				saveFile << std::to_string(readFrame->Frame[(y * qualityInfo->Width + x) * 3 + 1]) + " ";
				saveFile << std::to_string(readFrame->Frame[(y * qualityInfo->Width + x) * 3 + 2]) + ", ";
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
		qualityInfo->Height = 30;
		qualityInfo->Width = 60;
		qualityInfo->FrameRate = new FrameRate(1, 1);
		FrameEnhancerBase* enhancer;
		enhancer = new InterpolationFrameEnhancer(frameReader, qualityInfo);
		auto readFrame = enhancer->ReadNextEnhancedFrame();

		std::ofstream saveFile;
		saveFile.open("InterpolationFrameEnhTest.txt");

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
};
