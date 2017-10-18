#include "FilmUpperController.h"

FilmUpperController::FilmUpperController() {
	_viewModel = new FilmUpperViewModel();
}

FilmUpperController::~FilmUpperController() {
	delete _viewModel;
}