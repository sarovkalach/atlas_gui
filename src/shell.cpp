#include "shell.h"

//------------------ SimpleArtificialShell

void SimpleArtificialShell::chooseFileForEditing(){
	QString str = QFileDialog::getOpenFileName(0, "Choose in-file");
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

	setLayout(mL_);
}
