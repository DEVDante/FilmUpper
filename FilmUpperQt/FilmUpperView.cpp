#include "FilmUpperView.h"

void FilmUpperView::setupText()
{
	this->setWindowTitle(QApplication::translate("FilmUpperViewClass", "FilmUpper", Q_NULLPTR));
	actionO_programie->setText(QApplication::translate("FilmUpperViewClass", "O programie", Q_NULLPTR));
	actionOtw_rz_plik_wej_ciowy->setText(QApplication::translate("FilmUpperViewClass", "Otw\303\263rz plik wej\305\233ciowy", Q_NULLPTR));
	actionOtw_rz_plik_wyj_ciowy->setText(QApplication::translate("FilmUpperViewClass", "Wybierz plik wyj\305\233ciowy", Q_NULLPTR));
	actionZamknij->setText(QApplication::translate("FilmUpperViewClass", "Zamknij", Q_NULLPTR));
	inputButton->setText(QApplication::translate("FilmUpperViewClass", "Plik wej\305\233ciowy", Q_NULLPTR));
	outputButton->setText(QApplication::translate("FilmUpperViewClass", "Plik wyj\305\233ciowy", Q_NULLPTR));
	scalingLabel->setText(QApplication::translate("FilmUpperViewClass", "Algorytm skalowania", Q_NULLPTR));
	fpsLabel->setText(QApplication::translate("FilmUpperViewClass", "Algorytm zwi\304\231kszania il. klatek na sekunde", Q_NULLPTR));
	processButton->setText(QApplication::translate("FilmUpperViewClass", "Przetw\303\263rz", Q_NULLPTR));
	resGBox->setTitle(QApplication::translate("FilmUpperViewClass", "Rozdzielczo\305\233\304\207", Q_NULLPTR));
	multLabel->setText(QApplication::translate("FilmUpperViewClass", "x", Q_NULLPTR));
	fpsGBox->setTitle(QApplication::translate("FilmUpperViewClass", "Il. klatek na sekunde", Q_NULLPTR));
	menuPlik->setTitle(QApplication::translate("FilmUpperViewClass", "Plik", Q_NULLPTR));
	menuInfo->setTitle(QApplication::translate("FilmUpperViewClass", "Info", Q_NULLPTR));

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

void FilmUpperView::openInputFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Otw\303\263rz plik wideo"), NULL, tr("Pliki wideo (*.mp4 *.avi *.webm)"));
	int fps = 1;
	try {
		if (filename != "")
			fps = _controler->getFrameRate(filename.toStdString());
	}
	catch(std::exception e)
	{
		emit unexpectedError(e.what());
	}
	emit openedInFile(filename, fps);
}

void FilmUpperView::openOutputFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Zapisz plik wideo"), NULL, tr("Pliki wideo (*.mp4 *.avi *.webm)"));
	emit openedOutFile(filename);
}

void FilmUpperView::changeInValues(QString fname, int fps)
{
	inTBox->setText(fname);
	fpsSBox->setMinimum(fps);
	fpsSBox->setValue(fpsSBox->minimum());
}

void FilmUpperView::changeOutValues(QString fname)
{
	outTBox->setText(fname);
}

void FilmUpperView::changeRes(int index)
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

void FilmUpperView::showAbout()
{
	QMessageBox about;
	about.setWindowTitle("O programie");
	about.setText("FilmUpper \nProgram do polepszania jakości plików wideo \n\nKamil Rutkowski, Jakub Rup");
	about.exec();
}

void FilmUpperView::showError(std::string s)
{
	QMessageBox about;
	about.setWindowTitle("Błąd");
	about.setText(QString::fromStdString(s));
	about.exec();
}

void FilmUpperView::process()
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
	if (quality->FrameRate->num == _controler->getFrameRate(inTBox->text().toStdString()))
		fpsEnh = new NOPFpsEnhancerHeader();

	emit processStarted();
	try
	{
		_duration = _controler->getVideoDuration(inTBox->text().toStdString());
		progressBar->setMaximum(_duration);

		auto worker = _controler->startProcess();
		_processThread = new QThread;
		worker->moveToThread(_processThread);
		connect(worker, &Worker::secondProcessed, this, &FilmUpperView::procesedSecondsUpdate);
		connect(worker, &Worker::processEnded, this, &FilmUpperView::processCompleted);
		connect(worker, &Worker::exceptionInProcess, this, &FilmUpperView::errorInProcess);
		connect(this, &FilmUpperView::start, worker, &Worker::startProcess);
		_processThread->start();
		emit start(new std::string(inTBox->text().toStdString()), new std::string(outTBox->text().toStdString()), frameEnh, fpsEnh, quality);
		//&FilmUpperController::startProcess, _controler, inTBox->text().toStdString(), "temp.avi", frameEnh, fpsEnh, quality);
		//_controler->startProcess(inTBox->text().toStdString(), "temp.avi", frameEnh, fpsEnh, quality);		
	}
	catch (const std::exception& e)
	{
		emit unexpectedError(e.what());
	}
}

void FilmUpperView::lockUI()
{
	menuBar->setDisabled(true);
	groupingFrame->setDisabled(true);
	scalingCBox->setDisabled(true);
	fpsCBox->setDisabled(true);
	processButton->setDisabled(true);
	resGBox->setDisabled(true);
	fpsGBox->setDisabled(true);
}

void FilmUpperView::unlockUI()
{
	menuBar->setEnabled(true);
	groupingFrame->setEnabled(true);
	scalingCBox->setEnabled(true);
	fpsCBox->setEnabled(true);
	processButton->setEnabled(true);
	resGBox->setEnabled(true);
	fpsGBox->setEnabled(true);
}

void FilmUpperView::procesedSecondsUpdate(int secondProcessed)
{
	progressBar->setValue(secondProcessed);
}

void FilmUpperView::processCompleted()
{
	_processThread->quit();
	_processThread->wait();
	delete _processThread;
	progressBar->setValue(0);

	

	emit processEnded();
}

void FilmUpperView::errorInProcess(std::exception * e)
{
	emit unexpectedError(e->what());
}

FilmUpperView::FilmUpperView(QWidget *parent)
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
	widthSBox->setMinimum(1);
	widthSBox->setMaximum(9999);
	heightSBox = new QSpinBox(resGBox);
	heightSBox->setGeometry(QRect(100, 20, 71, 22));
	heightSBox->setMinimum(1);
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
	QObject::connect(inputButton, &QPushButton::clicked, this, &FilmUpperView::openInputFile);
	QObject::connect(outputButton, &QPushButton::clicked, this, &FilmUpperView::openOutputFile);
	QObject::connect(actionOtw_rz_plik_wej_ciowy, &QAction::triggered, this, &FilmUpperView::openInputFile);
	QObject::connect(actionOtw_rz_plik_wyj_ciowy, &QAction::triggered, this, &FilmUpperView::openOutputFile);
	QObject::connect(actionZamknij, &QAction::triggered, this, &FilmUpperView::close);
	QObject::connect(actionO_programie, &QAction::triggered, this, &FilmUpperView::showAbout);
	QObject::connect(this, &FilmUpperView::openedInFile, this, &FilmUpperView::changeInValues);
	QObject::connect(this, &FilmUpperView::openedOutFile, this, &FilmUpperView::changeOutValues);
	QObject::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FilmUpperView::changeRes);
	QObject::connect(processButton, &QPushButton::clicked, this, &FilmUpperView::process);
	QObject::connect(this, &FilmUpperView::processStarted, this, &FilmUpperView::lockUI);
	QObject::connect(this, &FilmUpperView::processEnded, this, &FilmUpperView::unlockUI);
	QObject::connect(this, &FilmUpperView::unexpectedError, this, &FilmUpperView::showError);


	setupText();

	_controler = new FilmUpperController();
	/*connect(_controler, SIGNAL(secondProcessed), this, SLOT(procesedSecondsUpdate));
	connect(_controler, SIGNAL(processEnded), this, SLOT(processCompleted));
	connect(_controler, SIGNAL(exceptionInProcess), this, SLOT(errorInProcess));*/
}

FilmUpperView::~FilmUpperView() {

	delete _controler;
}
