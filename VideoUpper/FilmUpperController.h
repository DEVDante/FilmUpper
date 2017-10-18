#pragma once
#include "FilmUpperViewModel.h"

class FilmUpperController {
public:
	FilmUpperController(FilmUpperViewModel* vm);
	~FilmUpperController();	
private:
	FilmUpperViewModel* _viewModel;
	void startProcess();
};