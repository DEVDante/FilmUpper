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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilmUpperViewClass
{
public:
    QAction *actionO_programie;
    QAction *actionOtw_rz_plik_wej_ciowy;
    QAction *actionOtw_rz_plik_wyj_ciowy;
    QAction *actionZamknij;
    QWidget *centralWidget;
    QFrame *groupingFrame;
    QLineEdit *inTBox;
    QLineEdit *outTBox;
    QPushButton *inputButton;
    QPushButton *outputButton;
    QLabel *scalingLabel;
    QLabel *fpsLabel;
    QPushButton *pushButton;
    QGroupBox *resGBox;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLabel *multLabel;
    QComboBox *comboBox;
    QGroupBox *fpsGBox;
    QSpinBox *fpsSBox;
    QProgressBar *progressBar;
    QComboBox *scalingCBox;
    QComboBox *fpsCBox;
    QMenuBar *menuBar;
    QMenu *menuPlik;
    QMenu *menuInfo;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *FilmUpperViewClass)
    {
        if (FilmUpperViewClass->objectName().isEmpty())
            FilmUpperViewClass->setObjectName(QStringLiteral("FilmUpperViewClass"));
        FilmUpperViewClass->resize(812, 275);
        actionO_programie = new QAction(FilmUpperViewClass);
        actionO_programie->setObjectName(QStringLiteral("actionO_programie"));
        actionOtw_rz_plik_wej_ciowy = new QAction(FilmUpperViewClass);
        actionOtw_rz_plik_wej_ciowy->setObjectName(QStringLiteral("actionOtw_rz_plik_wej_ciowy"));
        actionOtw_rz_plik_wyj_ciowy = new QAction(FilmUpperViewClass);
        actionOtw_rz_plik_wyj_ciowy->setObjectName(QStringLiteral("actionOtw_rz_plik_wyj_ciowy"));
        actionZamknij = new QAction(FilmUpperViewClass);
        actionZamknij->setObjectName(QStringLiteral("actionZamknij"));
        centralWidget = new QWidget(FilmUpperViewClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupingFrame = new QFrame(centralWidget);
        groupingFrame->setObjectName(QStringLiteral("groupingFrame"));
        groupingFrame->setGeometry(QRect(10, 10, 791, 71));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupingFrame->sizePolicy().hasHeightForWidth());
        groupingFrame->setSizePolicy(sizePolicy);
        groupingFrame->setFrameShape(QFrame::Box);
        groupingFrame->setFrameShadow(QFrame::Sunken);
        inTBox = new QLineEdit(groupingFrame);
        inTBox->setObjectName(QStringLiteral("inTBox"));
        inTBox->setGeometry(QRect(10, 10, 681, 20));
        outTBox = new QLineEdit(groupingFrame);
        outTBox->setObjectName(QStringLiteral("outTBox"));
        outTBox->setGeometry(QRect(10, 40, 681, 20));
        inputButton = new QPushButton(groupingFrame);
        inputButton->setObjectName(QStringLiteral("inputButton"));
        inputButton->setGeometry(QRect(694, 9, 91, 22));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inputButton->sizePolicy().hasHeightForWidth());
        inputButton->setSizePolicy(sizePolicy1);
        outputButton = new QPushButton(groupingFrame);
        outputButton->setObjectName(QStringLiteral("outputButton"));
        outputButton->setGeometry(QRect(694, 39, 91, 22));
        sizePolicy1.setHeightForWidth(outputButton->sizePolicy().hasHeightForWidth());
        outputButton->setSizePolicy(sizePolicy1);
        scalingLabel = new QLabel(centralWidget);
        scalingLabel->setObjectName(QStringLiteral("scalingLabel"));
        scalingLabel->setGeometry(QRect(20, 90, 141, 16));
        fpsLabel = new QLabel(centralWidget);
        fpsLabel->setObjectName(QStringLiteral("fpsLabel"));
        fpsLabel->setGeometry(QRect(20, 140, 211, 16));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(714, 92, 81, 141));
        resGBox = new QGroupBox(centralWidget);
        resGBox->setObjectName(QStringLiteral("resGBox"));
        resGBox->setGeometry(QRect(389, 100, 181, 91));
        spinBox = new QSpinBox(resGBox);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(10, 20, 71, 22));
        spinBox_2 = new QSpinBox(resGBox);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(100, 20, 71, 22));
        multLabel = new QLabel(resGBox);
        multLabel->setObjectName(QStringLiteral("multLabel"));
        multLabel->setGeometry(QRect(88, 21, 16, 16));
        comboBox = new QComboBox(resGBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(10, 60, 161, 22));
        fpsGBox = new QGroupBox(centralWidget);
        fpsGBox->setObjectName(QStringLiteral("fpsGBox"));
        fpsGBox->setGeometry(QRect(580, 100, 120, 91));
        fpsSBox = new QSpinBox(fpsGBox);
        fpsSBox->setObjectName(QStringLiteral("fpsSBox"));
        fpsSBox->setGeometry(QRect(10, 40, 101, 22));
        fpsSBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(10, 210, 691, 23));
        progressBar->setValue(0);
        scalingCBox = new QComboBox(centralWidget);
        scalingCBox->setObjectName(QStringLiteral("scalingCBox"));
        scalingCBox->setGeometry(QRect(20, 110, 351, 22));
        fpsCBox = new QComboBox(centralWidget);
        fpsCBox->setObjectName(QStringLiteral("fpsCBox"));
        fpsCBox->setGeometry(QRect(20, 160, 351, 22));
        FilmUpperViewClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FilmUpperViewClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 812, 21));
        menuPlik = new QMenu(menuBar);
        menuPlik->setObjectName(QStringLiteral("menuPlik"));
        menuInfo = new QMenu(menuBar);
        menuInfo->setObjectName(QStringLiteral("menuInfo"));
        FilmUpperViewClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FilmUpperViewClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FilmUpperViewClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuPlik->menuAction());
        menuBar->addAction(menuInfo->menuAction());
        menuPlik->addAction(actionOtw_rz_plik_wej_ciowy);
        menuPlik->addAction(actionOtw_rz_plik_wyj_ciowy);
        menuPlik->addSeparator();
        menuPlik->addAction(actionZamknij);
        menuInfo->addAction(actionO_programie);

        retranslateUi(FilmUpperViewClass);

        QMetaObject::connectSlotsByName(FilmUpperViewClass);
    } // setupUi

    void retranslateUi(QMainWindow *FilmUpperViewClass)
    {
        FilmUpperViewClass->setWindowTitle(QApplication::translate("FilmUpperViewClass", "FilmUpper", Q_NULLPTR));
        actionO_programie->setText(QApplication::translate("FilmUpperViewClass", "O programie", Q_NULLPTR));
        actionOtw_rz_plik_wej_ciowy->setText(QApplication::translate("FilmUpperViewClass", "Otw\303\263rz plik wej\305\233ciowy", Q_NULLPTR));
        actionOtw_rz_plik_wyj_ciowy->setText(QApplication::translate("FilmUpperViewClass", "Otw\303\263rz plik wyj\305\233ciowy", Q_NULLPTR));
        actionZamknij->setText(QApplication::translate("FilmUpperViewClass", "Zamknij", Q_NULLPTR));
        inputButton->setText(QApplication::translate("FilmUpperViewClass", "Plik wej\305\233ciowy", Q_NULLPTR));
        outputButton->setText(QApplication::translate("FilmUpperViewClass", "Plik wyj\305\233ciowy", Q_NULLPTR));
        scalingLabel->setText(QApplication::translate("FilmUpperViewClass", "Algorytm skalowania", Q_NULLPTR));
        fpsLabel->setText(QApplication::translate("FilmUpperViewClass", "Algorytm zwi\304\231kszania il. klatek na sekunde", Q_NULLPTR));
        pushButton->setText(QApplication::translate("FilmUpperViewClass", "Przetw\303\263rz", Q_NULLPTR));
        resGBox->setTitle(QApplication::translate("FilmUpperViewClass", "Rozdzielczo\305\233\304\207", Q_NULLPTR));
        multLabel->setText(QApplication::translate("FilmUpperViewClass", "x", Q_NULLPTR));
        fpsGBox->setTitle(QApplication::translate("FilmUpperViewClass", "Il. klatek na sekunde", Q_NULLPTR));
        menuPlik->setTitle(QApplication::translate("FilmUpperViewClass", "Plik", Q_NULLPTR));
        menuInfo->setTitle(QApplication::translate("FilmUpperViewClass", "Info", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FilmUpperViewClass: public Ui_FilmUpperViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILMUPPERQT_H
