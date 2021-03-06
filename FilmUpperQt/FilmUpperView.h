#pragma once

#include <QtWidgets/QMainWindow>
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
#include <QThread>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QProcess>
#include <QFileInfo>
#include <QUuid>
#include <QFile>
#include <stdlib.h>

#include "FilmUpperController.h"
#include "IFrameEnhancerHeader.h"
#include "IFpsEnhancerHeader.h"
#include "FrameEnhancerBase.h"
#include "BiCubicFrameEnhancerHeader.h"
#include "InterlaceFpsEnhancerHeader.h"
#include "InterpolationFpsEnhancerHeader.h"
#include "InterpolationFrameEnhancerHeader.h"
#include "MaskFrameEnhancerHeader.h"
#include "NNFrameEnhancerHeader.h"
#include "NOPFpsEnhancerHeader.h"

class FilmUpperView : public QMainWindow
{
	Q_OBJECT

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
	QPushButton *processButton;
	QGroupBox *resGBox;
	QSpinBox *widthSBox;
	QSpinBox *heightSBox;
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

	FilmUpperView(QWidget *parent = Q_NULLPTR);
	~FilmUpperView();
	void setupText();

private:
	//Ui::FilmUpperViewClass ui;
	FilmUpperController* _controler;
	uint64_t _duration;

	QThread* _processThread;

signals:
	void openedInFile(QString fname, int fps);
	void openedOutFile(QString fname);
	void unexpectedError(std::string);
	void processStarted();
	void processEnded();
	void start(std::string* fileSourcePath, std::string* fileTargetPath, IFrameEnhancerHeader* frameEnhancerHeader, IFpsEnhancerHeader* fpsEnhancerHeader, FilmQualityInfo* targetQuality);

private slots:
	void openInputFile();
	void openOutputFile();
	void changeInValues(QString fname, int fps);
	void changeOutValues(QString fname);
	void changeRes(int index);
	void showAbout();
	void showError(std::string s);
	void process();
	void lockUI();
	void unlockUI();
	void procesedSecondsUpdate(int secondProcessed);
	void processCompleted();
	void errorInProcess(std::exception* e);
};
