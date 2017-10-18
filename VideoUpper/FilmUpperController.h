#pragma once
#include "FilmUpperViewModel.h"

class FilmUpperController {
public:
	FilmUpperController();
	virtual ~FilmUpperController();
private:
	FilmUpperViewModel* _viewModel;
};