#pragma once

class FilmUpperView {
public:
	FilmUpperView(FilmUpperViewModel* viewModel);
	virtual ~FilmUpperView();
private:
	FilmUpperViewModel* _viewModel;
};