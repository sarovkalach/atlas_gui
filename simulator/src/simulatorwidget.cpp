#include "simulatorwidget.h"

// ----------------------------- TextDialog

TextDialog::TextDialog(QString &text, QWidget *pwg) : QDialog(pwg), text_(text){
	mL_ = new QVBoxLayout;
	editor_ = new QTextEdit;
	okButton_ = new QPushButton(tr("OK"));

	editor_->setText(text_);

	mL_->addWidget(editor_);
	mL_->addWidget(okButton_);
	setLayout(mL_);

	connect(okButton_, SIGNAL(clicked()), this, SLOT(pushOk()));
}

void TextDialog::pushOk(){
	text_ = editor_->toPlainText();
	accept();
}

// ----------------------------- SimulatorWidget

void SimulatorWidget::setAtlasModellingRelations(){
	model->setRelation(3, QSqlRelation("atlas.simulator", "id", "name"));
	QString filter = QString("owner= %1 ").arg(owner);
	model->setFilter(filter);
}

void SimulatorWidget::initAtlasModellingModelView(){
	model = new SimulatorTableModel;
	view = new SimulatorTableView;
	view->setEditTriggers(QTableView::NoEditTriggers);  // read only view
	view->setModel(model);
	view->horizontalHeader()->setStretchLastSection(true);

	setAtlasModellingRelations();

	model->setHeaderData(2, Qt::Horizontal, tr("Date"));
	model->setHeaderData(3, Qt::Horizontal, tr("Model Name"));
	model->setHeaderData(4, Qt::Horizontal, tr("InFile"));
	model->setHeaderData(5, Qt::Horizontal, tr("OutFile"));
	model->setHeaderData(6, Qt::Horizontal, tr("Desctiption"));
	model->setHeaderData(7, Qt::Horizontal, tr("Progress"));
	model->setHeaderData(8, Qt::Horizontal, tr("SlurmID"));
	model->setHeaderData(9, Qt::Horizontal, tr("Catalog"));

	view->hideColumn(0);
	view->hideColumn(1);
}

void SimulatorWidget::showTextEdit(){
	TextDialog::createTextDialog(description_ , this);
}

void SimulatorWidget::startExpirement() {
	//запустить симулятор
	qDebug() << "Trying to start Experiment";
	if (starter_.startSimulator(shell->get_inFileLineEditor()->text(), modellingComboBox->currentText(), description_)){

		//ОТДЕЛЬНО ПРОПИСАТЬ ЗАПУСК НА WINDOWS!
		//...

		//запустить обновление БД и показать QProgressBar
		progressBar_->setTimerInterval(updateInterval_);
		progressBar_->initTimer();
		showModellingtableModel(owner);
		description_ = "";

		stopButton->setDisabled(false);
	}
}

void SimulatorWidget::showSAH(){
	if (shell->isVisible()){
		shell->hide();
	}
	else{
		shell->show();
	}
}

void SimulatorWidget::initButtons(){
	buttonsLayout = new QHBoxLayout;
	startButton = new QPushButton;
	stopButton = new QPushButton;
	descriptionButton_ = new QPushButton;
	editConfigButton = new QPushButton;

	startButton->setDisabled(true);
	stopButton->setDisabled(true);

	startButton->setText(tr("Start"));
	stopButton->setText(tr("Stop"));
	descriptionButton_->setText(tr("Set description..."));
	editConfigButton->setText(tr("Notepad"));

	buttonsLayout->addWidget(startButton);
	buttonsLayout->addWidget(stopButton);
	buttonsLayout->addWidget(descriptionButton_);
	buttonsLayout->addWidget(editConfigButton);

	connect(startButton, SIGNAL(clicked()), this, SLOT(startExpirement()));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stopExperiment()));
	connect(descriptionButton_, SIGNAL(clicked()), this, SLOT(showTextEdit()));
	connect(editConfigButton, SIGNAL(clicked()), this, SLOT(showSAH()));
}

void SimulatorWidget::initAllSimulatorsFromDB(){
	QSqlRelationalTableModel simulatorTable;
	modellingComboBox = new QComboBox;

	simulatorTable.setTable("atlas.simulator");
	simulatorTable.select();

	for (int i=0; i < simulatorTable.rowCount(); i++ )
		 modellingComboBox->addItem(simulatorTable.data(simulatorTable.index(i,2)).toString());

	modellingComboBox->setCurrentIndex(0);
}

SimulatorWidget::SimulatorWidget(int ownerIn, QWidget* pwgt) : QWidget(pwgt), owner(ownerIn), starter_(owner)
{
	shell = new SimpleArtificialShell(this);
	shell->set_defaultDir(starter_.init());
	initAtlasModellingModelView();
	initButtons();
	initAllSimulatorsFromDB();

    modellingLabel = new QLabel;

	progressBar_ = new ProgressBar(processID_);

	modellingLabel->setText(tr("Set simulator"));

	modellingVLayout = new QVBoxLayout;
	splitter_ = new QSplitter;

	splitter_->setOrientation(Qt::Vertical);
	splitter_->addWidget(shell);
	splitter_->addWidget(view);
	splitter_->setStretchFactor(1, 100);


    modellingVLayout->addWidget(modellingLabel);
	modellingVLayout->addWidget(modellingComboBox);
	modellingVLayout->addLayout(buttonsLayout);
	modellingVLayout->addWidget(splitter_);
	modellingVLayout->addWidget(progressBar_);
	progressBar_->hide();
	shell->hide();

	setLayout(modellingVLayout);

    connect(shell->get_inFileLineEditor(), SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
	                //установка таймера для периодического обновления таблицы.
	timerForUpdateTable_ = new QTimer;
	timerForUpdateTable_->setInterval(updateInterval_);
	connect(timerForUpdateTable_, SIGNAL(timeout()), model, SLOT(select()));
	timerForUpdateTable_->start();
}


void SimulatorWidget::stopExperiment() {
	qDebug() << "Stop Experiment";

	showModellingtableModel(owner);

	starter_.cancel();

	//остановить обновление строки прогресса из БД и скрыть QProgressBar
	progressBar_->deleteTimer();
	stopButton->setDisabled(true);
}

void SimulatorWidget::showModellingtableModel(const int idOwner)
{	
	if (owner != idOwner){
		owner = idOwner;
		qDebug()<<"owner = "<<owner;
		setAtlasModellingRelations();
	}

	model->select();
}

void SimulatorWidget::enableButtons(const QString text) {
    startButton->setEnabled(true);
}
