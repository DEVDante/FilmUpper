#include "FilmUpperQt.h"

FilmUpperQt::FilmUpperQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	_controler = new FilmUpperController();
}

FilmUpperQt::~FilmUpperQt() {
	delete _controler;
}
