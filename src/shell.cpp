#include "shell.h"
#include "QDebug"

SimpleArtificialShell::SimpleArtificialShell(QWidget *obj) : QWidget(obj){
    mL_ = new QVBoxLayout;

    buttonLayout_ = new QHBoxLayout;
    openFileButton_ = new QPushButton("Open in-file...");
    inFileLab_ = new QLineEdit("");
    inFileLab_->setReadOnly(true);

    buttonLayout_->addWidget(inFileLab_, 1000);
    buttonLayout_->addWidget(openFileButton_);

    mL_->addLayout(buttonLayout_);

    connect(openFileButton_, SIGNAL(clicked()), this, SLOT(chooseFileForEditing()));			//связывает открытие диалогового окна с кнопкой "Open in-file..."
    connect(inFileLab_, SIGNAL(textChanged(QString)), this, SLOT(openFileForEditing(QString)));	//связывает изменение в строке с in-file-ом с открытием программы в текстовом редакторе
    //connect(inFileLab_, SIGNAL(textChanged(QString)), this->parentWidget(), SLOT(this->parentWidget()->enableButtons(QString )));
    qDebug() << this->parentWidget() << endl;
    setLayout(mL_);
}


//------------------ SimpleArtificialShell
void SimpleArtificialShell::chooseFileForEditing(){
//	QString str = QFileDialog::getOpenFileName(0, "Choose in-file");
	QString str = QFileDialog::getSaveFileName(0, "Choose in-file");

	ifstream fin;
	fin.open(str.toStdString().c_str(), ios::in);
	if (!fin.is_open()){
		//не можем открыть файл, т.к. его нет => создаем пустой файл.
		fin.close();

		ofstream fout;
		fout.open(str.toStdString().c_str(), ios::out);
		if (!fout.is_open()){
			QMessageBox::critical(0, "Attention", "Can\'t create file with name:\n" + str + "\nmay be you forgot to create folders?");
		}

		fout.close();
	}
	else{
		fin.close();
	}

	inFileLab_->setText(str);
}
void SimpleArtificialShell::openFileForEditing(QString str){
	if (fileToTab_ != nullptr){
		//удалить
		mL_->removeWidget(fileToTab_);
		delete fileToTab_;
		fileToTab_ = nullptr;
	}
	fileToTab_ = new fttw::OutSideWidget(str.toStdString());
	mL_->addWidget(fileToTab_);
}
void SimpleArtificialShell::openFileForEditingWithLineEditor(const QString& str){
	if (str != inFileLab_->text()){
		inFileLab_->setText(str);
	}
}


