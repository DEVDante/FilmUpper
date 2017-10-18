#include "FilmUpperController.h"
#include "FilmUpperView.h"

int main() {
	FilmUpperViewModel* viewModel = new FilmUpperViewModel();
	FilmUpperController* controller = new FilmUpperController(viewModel);
	FilmUpperView* view = new FilmUpperView(viewModel);

	delete controller;
	delete view;
	delete viewModel;
}