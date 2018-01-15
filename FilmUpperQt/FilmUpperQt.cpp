#include "FilmUpperQt.h"

void FilmUpperQt::setupText()
{
	this->setWindowTitle(QApplication::translate("FilmUpperQtClass", "FilmUpper", Q_NULLPTR));
	actionO_programie->setText(QApplication::translate("FilmUpperQtClass", "O programie", Q_NULLPTR));
	actionOtw_rz_plik_wej_ciowy->setText(QApplication::translate("FilmUpperQtClass", "Otw\303\263rz plik wej\305\233ciowy", Q_NULLPTR));
	actionOtw_rz_plik_wyj_ciowy->setText(QApplication::translate("FilmUpperQtClass", "Wybierz plik wyj\305\233ciowy", Q_NULLPTR));
	actionZamknij->setText(QApplication::translate("FilmUpperQtClass", "Zamknij", Q_NULLPTR));
	inputButton->setText(QApplication::translate("FilmUpperQtClass", "Plik wej\305\233ciowy", Q_NULLPTR));
	outputButton->setText(QApplication::translate("FilmUpperQtClass", "Plik wyj\305\233ciowy", Q_NULLPTR));
	scalingLabel->setText(QApplication::translate("FilmUpperQtClass", "Algorytm skalowania", Q_NULLPTR));
	fpsLabel->setText(QApplication::translate("FilmUpperQtClass", "Algorytm zwi\304\231kszania il. klatek na sekunde", Q_NULLPTR));
	processButton->setText(QApplication::translate("FilmUpperQtClass", "Przetw\303\263rz", Q_NULLPTR));
	resGBox->setTitle(QApplication::translate("FilmUpperQtClass", "Rozdzielczo\305\233\304\207", Q_NULLPTR));
	multLabel->setText(QApplication::translate("FilmUpperQtClass", "x", Q_NULLPTR));
	fpsGBox->setTitle(QApplication::translate("FilmUpperQtClass", "Il. klatek na sekunde", Q_NULLPTR));
	menuPlik->setTitle(QApplication::translate("FilmUpperQtClass", "Plik", Q_NULLPTR));
	menuInfo->setTitle(QApplication::translate("FilmUpperQtClass", "Info", Q_NULLPTR));

	QStringList scalingAlgs;
	scalingAlgs << "Interpolacja dwusześcienna" << "Najbliższy sąsiad" << "Interpolacja dwuliniowa" << "Interpolacja dwuliniowa z filtrem ostrości";

	QStringList fpsAlgs;
	fpsAlgs << "Interpolacja" << "Przeplot";

	QStringList resList;
	resList << "720p" << "1080p" << "2K" << "4K";

	scalingCBox->addItems(scalingAlgs);
	fpsCBox->addItems(fpsAlgs);
	comboBox->addItems(resList);
}

void FilmUpperQt::openInputFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Otw\303\263rz plik wideo"), NULL, tr("Pliki wideo (*.mp4 *.avi *.webm)"));
	int fps = 1;
	if (filename != "")
		fps = _controler->getFrameRate(filename.toStdString());
	emit openedInFile(filename, fps);
}

void FilmUpperQt::openOutputFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Zapisz plik wideo"), NULL, tr("Pliki wideo (*.mp4 *.avi *.webm)"));
	emit openedOutFile(filename);
}

void FilmUpperQt::changeInValues(QString fname)
{
	inTBox->setText(fname);
}

void FilmUpperQt::changeOutValues(QString fname)
{
	outTBox->setText(fname);
}

void FilmUpperQt::changeFpsValues(int fps)
{
	fpsSBox->setMinimum(fps);
	fpsSBox->setValue(fpsSBox->minimum());
}

void FilmUpperQt::changeRes(int index)
{
	int width;
	int height;
	switch (index)
	{
	case 0:
		width = 1280;
		height = 720;
		break;
	case 1:
		width = 1920;
		height = 1080;
		break;
	case 2:
		width = 2048;
		height = 1152;
		break;
	case 3:
		width = 4096;
		height = 2304;
		break;
	default:
		break;
	}

	widthSBox->setValue(width);
	heightSBox->setValue(height);
}

void FilmUpperQt::getVideoData(QString fname)
{

}

void FilmUpperQt::showAbout()
{
	QMessageBox about;
	about.setWindowTitle("O programie");
	about.setText("FilmUpper \nProgram do polepszania jakości plików wideo \n\nKamil Rutkowski, Jakub Rup");
	about.exec();
}

void FilmUpperQt::showError(std::string s)
{
	QMessageBox about;
	about.setWindowTitle("Błąd");
	about.setText(QString::fromStdString(s));
	about.exec();
}

void FilmUpperQt::process()
{
	IFrameEnhancerHeader *frameEnh;
	IFpsEnhancerHeader *fpsEnh;
	FilmQualityInfo *quality;

	quality = new FilmQualityInfo();
	quality->Width = widthSBox->value();
	quality->Height = heightSBox->value();
	quality->FrameRate = new FrameRate(fpsSBox->value(), 1);

	switch (scalingCBox->currentIndex())
	{
	case 0:
		frameEnh = new BiCubicFrameEnhancerHeader();
		break;
	case 1:
		frameEnh = new NNFrameEnhancerHeader();
		break;
	case 2:
		frameEnh = new InterpolationFrameEnhancerHeader();
		break;
	case 3:
		frameEnh = new MaskFrameEnhancerHeader();
		break;
	default:
		break;
	}

	switch (fpsCBox->currentIndex())
	{
	case 0:
		fpsEnh = new InterpolationFpsEnhancerHeader();
		break;
	case 1:
		fpsEnh = new InterlaceFpsEnhancerHeader();
		break;
	default:
		break;
	}

	if (inTBox->text() == "" || outTBox->text() =="")
	{
		emit unexpectedError("Nazwy plików źródłowego i docelowego nie mogą być puste!");
		return;
	}

	if (inTBox->text() == outTBox->text())
	{
		emit unexpectedError("Nazwa i ścieżka pliku wejściowego nie może byc taka sama jak pliku wyjściowego!");
		return;
	}

	emit processStarted();
	try
	{
		_duration = _controler->getVideoDuration(inTBox->text().toStdString());

		_controler->startProcess(inTBox->text().toStdString(), "temp.avi", frameEnh, fpsEnh, quality);

		QProcess muxProcess(this);
		QString program = "ffmpeg.exe";
		QStringList arguments;
		arguments << "-i" << "temp.avi" << "-i" << inTBox->text() << "-c copy -map 0:v:0 -map 1:a:0 -shortest" << outTBox->text();

		muxProcess.start(program, arguments);
	}
	catch (const std::exception& e)
	{
		emit unexpectedError(e.what());
	}
	emit processEnded();

	delete frameEnh;
	delete fpsEnh;
	delete quality;
}

void FilmUpperQt::lockUI()
{
	menuBar->setDisabled(true);
	groupingFrame->setDisabled(true);
	scalingCBox->setDisabled(true);
	fpsCBox->setDisabled(true);
	processButton->setDisabled(true);
	resGBox->setDisabled(true);
	fpsGBox->setDisabled(true);
}

void FilmUpperQt::unlockUI()
{
	menuBar->setEnabled(true);
	groupingFrame->setEnabled(true);
	scalingCBox->setEnabled(true);
	fpsCBox->setEnabled(true);
	processButton->setEnabled(true);
	resGBox->setEnabled(true);
	fpsGBox->setEnabled(true);
}

FilmUpperQt::FilmUpperQt(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	this->resize(812, 274);
	this->setFixedSize(size());

	actionO_programie = new QAction(this);
	actionOtw_rz_plik_wej_ciowy = new QAction(this);
	actionOtw_rz_plik_wyj_ciowy = new QAction(this);
	actionZamknij = new QAction(this);
	centralWidget = new QWidget(this);
	groupingFrame = new QFrame(centralWidget);
	groupingFrame->setGeometry(QRect(10, 10, 791, 71));
	QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(groupingFrame->sizePolicy().hasHeightForWidth());
	groupingFrame->setSizePolicy(sizePolicy);
	groupingFrame->setFrameShape(QFrame::Box);
	groupingFrame->setFrameShadow(QFrame::Sunken);
	inTBox = new QLineEdit(groupingFrame);
	inTBox->setGeometry(QRect(10, 10, 681, 20));
	outTBox = new QLineEdit(groupingFrame);
	outTBox->setGeometry(QRect(10, 40, 681, 20));
	inputButton = new QPushButton(groupingFrame);
	inputButton->setGeometry(QRect(694, 9, 91, 22));
	QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(inputButton->sizePolicy().hasHeightForWidth());
	inputButton->setSizePolicy(sizePolicy1);
	outputButton = new QPushButton(groupingFrame);
	outputButton->setGeometry(QRect(694, 39, 91, 22));
	sizePolicy1.setHeightForWidth(outputButton->sizePolicy().hasHeightForWidth());
	outputButton->setSizePolicy(sizePolicy1);
	scalingLabel = new QLabel(centralWidget);
	scalingLabel->setGeometry(QRect(20, 90, 141, 16));
	fpsLabel = new QLabel(centralWidget);
	fpsLabel->setGeometry(QRect(20, 140, 211, 16));
	processButton = new QPushButton(centralWidget);
	processButton->setGeometry(QRect(714, 92, 81, 141));
	resGBox = new QGroupBox(centralWidget);
	resGBox->setGeometry(QRect(389, 100, 181, 91));
	widthSBox = new QSpinBox(resGBox);
	widthSBox->setGeometry(QRect(10, 20, 71, 22));
	widthSBox->setMaximum(9999);
	heightSBox = new QSpinBox(resGBox);
	heightSBox->setGeometry(QRect(100, 20, 71, 22));
	heightSBox->setMaximum(9999);
	multLabel = new QLabel(resGBox);
	multLabel->setGeometry(QRect(88, 21, 16, 16));
	comboBox = new QComboBox(resGBox);
	comboBox->setGeometry(QRect(10, 60, 161, 22));
	fpsGBox = new QGroupBox(centralWidget);
	fpsGBox->setGeometry(QRect(580, 100, 120, 91));
	fpsSBox = new QSpinBox(fpsGBox);
	fpsSBox->setGeometry(QRect(10, 40, 101, 22));
	fpsSBox->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
	fpsSBox->setMaximum(144);
	fpsSBox->setMinimum(1);
	progressBar = new QProgressBar(centralWidget);
	progressBar->setGeometry(QRect(10, 210, 691, 23));
	progressBar->setValue(0);
	scalingCBox = new QComboBox(centralWidget);
	scalingCBox->setGeometry(QRect(20, 110, 351, 22));
	fpsCBox = new QComboBox(centralWidget);
	fpsCBox->setGeometry(QRect(20, 160, 351, 22));
	this->setCentralWidget(centralWidget);
	menuBar = new QMenuBar(this);
	menuBar->setGeometry(QRect(0, 0, 812, 21));
	menuPlik = new QMenu(menuBar);
	menuInfo = new QMenu(menuBar);
	this->setMenuBar(menuBar);
	mainToolBar = new QToolBar(this);
	this->addToolBar(Qt::TopToolBarArea, mainToolBar);

	//tab order
	QWidget::setTabOrder(inTBox, inputButton);
	QWidget::setTabOrder(inputButton, outTBox);
	QWidget::setTabOrder(outTBox, outputButton);
	QWidget::setTabOrder(outputButton, scalingCBox);
	QWidget::setTabOrder(scalingCBox, fpsCBox);
	QWidget::setTabOrder(fpsCBox, widthSBox);
	QWidget::setTabOrder(widthSBox, heightSBox);
	QWidget::setTabOrder(heightSBox, comboBox);
	QWidget::setTabOrder(comboBox, fpsSBox);
	QWidget::setTabOrder(fpsSBox, processButton);

	//menu setup
	menuBar->addAction(menuPlik->menuAction());
	menuBar->addAction(menuInfo->menuAction());
	menuPlik->addAction(actionOtw_rz_plik_wej_ciowy);
	menuPlik->addAction(actionOtw_rz_plik_wyj_ciowy);
	menuPlik->addSeparator();
	menuPlik->addAction(actionZamknij);
	menuInfo->addAction(actionO_programie);


	//connect slots
	QObject::connect(inputButton, &QPushButton::clicked, this, &FilmUpperQt::openInputFile);
	QObject::connect(outputButton, &QPushButton::clicked, this, &FilmUpperQt::openOutputFile);
	QObject::connect(actionOtw_rz_plik_wej_ciowy, &QAction::triggered, this, &FilmUpperQt::openInputFile);
	QObject::connect(actionOtw_rz_plik_wyj_ciowy, &QAction::triggered, this, &FilmUpperQt::openOutputFile);
	QObject::connect(actionZamknij, &QAction::triggered, this, &FilmUpperQt::close);
	QObject::connect(actionO_programie, &QAction::triggered, this, &FilmUpperQt::showAbout);
	QObject::connect(this, &FilmUpperQt::openedInFile, this, &FilmUpperQt::changeInValues);
	QObject::connect(this, &FilmUpperQt::openedOutFile, this, &FilmUpperQt::changeOutValues);
	QObject::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FilmUpperQt::changeRes);
	QObject::connect(processButton, &QPushButton::clicked, this, &FilmUpperQt::process);
	QObject::connect(this, &FilmUpperQt::processStarted, this, &FilmUpperQt::lockUI);
	QObject::connect(this, &FilmUpperQt::processEnded, this, &FilmUpperQt::unlockUI);
	QObject::connect(this, &FilmUpperQt::unexpectedError, this, &FilmUpperQt::showError);


	setupText();

	_controler = new FilmUpperController();
}

FilmUpperQt::~FilmUpperQt() {

	delete _controler;
}
