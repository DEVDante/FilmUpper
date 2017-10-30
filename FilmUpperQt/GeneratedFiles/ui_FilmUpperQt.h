/********************************************************************************
** Form generated from reading UI file 'FilmUpperQt.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILMUPPERQT_H
#define UI_FILMUPPERQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilmUpperQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FilmUpperQtClass)
    {
        if (FilmUpperQtClass->objectName().isEmpty())
            FilmUpperQtClass->setObjectName(QStringLiteral("FilmUpperQtClass"));
        FilmUpperQtClass->resize(600, 400);
        menuBar = new QMenuBar(FilmUpperQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FilmUpperQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FilmUpperQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FilmUpperQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FilmUpperQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FilmUpperQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FilmUpperQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FilmUpperQtClass->setStatusBar(statusBar);

        retranslateUi(FilmUpperQtClass);

        QMetaObject::connectSlotsByName(FilmUpperQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *FilmUpperQtClass)
    {
        FilmUpperQtClass->setWindowTitle(QApplication::translate("FilmUpperQtClass", "FilmUpperQt", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FilmUpperQtClass: public Ui_FilmUpperQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILMUPPERQT_H
