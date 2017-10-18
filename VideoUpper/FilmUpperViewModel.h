#pragma once
#include "IFpsEnchanterHeader.h"
#include "IFrameEnchanterHeader.h"
#include <string>

const int NUMBER_OFF_FPS_HEADERS = 5;
const int NUMBER_OFF_Frame_HEADERS = 5;


class FilmUpperViewModel {
public:
	FilmUpperViewModel() {}
	~FilmUpperViewModel() {}

	//Parameters
	std::string SelectedFile;
	IFpsEnchanterHeader* FpsEnchanterHeaders[NUMBER_OFF_FPS_HEADERS] = {};
	IFrameEnchanterHeader* FrameEnchanterHeaders[NUMBER_OFF_Frame_HEADERS] = {};
	IFpsEnchanterHeader* SelectedFpsEnchanterHeader;
	IFrameEnchanterHeader* SelectedFrameEnchanterHeader;
};