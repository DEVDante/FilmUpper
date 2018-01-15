#include "FilmUpperView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FilmUpperView w;
	w.show();
	return a.exec();
}
