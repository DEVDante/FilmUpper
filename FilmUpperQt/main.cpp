#include "FilmUpperQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FilmUpperQt w;
	w.show();
	return a.exec();
}
