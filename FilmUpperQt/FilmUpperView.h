#pragma once
#include "FilmUpperViewModel.h"

class FilmUpperView {
public:
	FilmUpperView(FilmUpperViewModel* viewModel);
	~FilmUpperView();
	void Show();
private:
	FilmUpperViewModel* _viewModel;
};