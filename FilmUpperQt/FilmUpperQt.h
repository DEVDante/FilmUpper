#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FilmUpperQt.h"
#include "FilmUpperController.h"

class FilmUpperQt : public QMainWindow
{
	Q_OBJECT

public:
	FilmUpperQt(QWidget *parent = Q_NULLPTR);
	~FilmUpperQt();

private:
	Ui::FilmUpperQtClass ui;

	FilmUpperController* _controler;
};
