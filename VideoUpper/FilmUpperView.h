#pragma once
#include "FilmUpperViewModel.h"

class FilmUpperView {
public:
	FilmUpperView(FilmUpperViewModel* viewModel);
	~FilmUpperView();
private:
	FilmUpperViewModel* _viewModel;
};