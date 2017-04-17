#include "../general_gui_files/include/startwindow.h"

void connectToTheDataBase(QApplication& app){
	qDebug() << "Sql Driver" << QSqlDatabase::drivers();
	QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	db.setHostName("85.143.2.188");
	db.setDatabaseName("atlas3d");
	db.setUserName("atlas3d");
	bool connectioncheck = db.open("atlas3d","");

	if (connectioncheck == true){
		qDebug() << "Connection to database established." << endl;
	} else {
		qDebug() << "Error for database " << db.databaseName() << " :" << db.lastError().text() << endl;
		QMessageBox::critical(Q_NULLPTR, app.translate("QMessageBoxDB", "Error"), app.translate("QMessageBoxDB", "Can\'t connect to the database."));
		exit(-1);
	}
}

StartWindow::StartWindow(QWidget *pwgt) : QDialog(pwgt) {

    translator = new QTranslator;
    languageComboBox = new QComboBox();
    languageComboBox->addItem("English");
    languageComboBox->addItem("Russian");

    languageLabel = new QLabel;
    languageLabel->setText("Choose Languge");

    startLayout = new QVBoxLayout;
    startLayout->addWidget(languageLabel);
    startLayout->addWidget(languageComboBox);

    okButton = new QPushButton;
    okButton->setText("Ok");
    cancelButton = new QPushButton;
    cancelButton->setText("Cancel");

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    startLayout->addLayout(buttonsLayout);
	setLayout(startLayout);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(startUp()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));

}


void StartWindow::setApp(QApplication *applictaion) {
	app = applictaion;
}


void StartWindow::startUp() {
	QString language = languageComboBox->currentText();
	setLanguage(language);
	accept();
}
void StartWindow::setLanguage(const QString lang) {
	if (lang == "Russian"){
		if ( !(translator->load(":/main_ru.qm")) ){
			qDebug()<<"Can\'t open file with name \":/main_ru.qm\"";
		}
	}
	else{
		if ( !(translator->load(":/main_en.qm")) ){
			qDebug()<<"Can\'t open file with name \":/main_en.qm\"";
		}
	}

    app->installTranslator(translator);
}
