#pragma once
#include "FilmUpperView.h"
#include "IFpsEnchanterHeader.h"
#include "IFrameEnchanterHeader.h"
#include <string>

const int NUMBER_OFF_FPS_HEADERS = 5;
const int NUMBER_OFF_Frame_HEADERS = 5;


class FilmUpperViewModel {
public:
	FilmUpperViewModel() { _view = new FilmUpperView(this); };
	~FilmUpperViewModel() { delete _view; }
	std::string SelectedFile;
	IFpsEnchanterHeader* FpsEnchanterHeaders[NUMBER_OFF_FPS_HEADERS] = {};
	IFrameEnchanterHeader* FrameEnchanterHeaders[NUMBER_OFF_Frame_HEADERS] = {};
	IFpsEnchanterHeader* SelectedFpsEnchanterHeader;
	IFrameEnchanterHeader* SelectedFrameEnchanterHeader;
	
private:
	FilmUpperView* _view;
};