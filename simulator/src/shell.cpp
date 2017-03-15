#include "shell.h"
#include "QDebug"

SimpleArtificialShell::SimpleArtificialShell(QWidget *obj) : QWidget(obj){
    mL_ = new QVBoxLayout;

    buttonLayout_ = new QHBoxLayout;
	openFileButton_ = new QPushButton(tr("Open in-file..."));
    inFileLab_ = new QLineEdit("");
    inFileLab_->setReadOnly(true);

    buttonLayout_->addWidget(inFileLab_, 1000);
    buttonLayout_->addWidget(openFileButton_);

    mL_->addLayout(buttonLayout_);

    connect(openFileButton_, SIGNAL(clicked()), this, SLOT(chooseFileForEditing()));			//связывает открытие диалогового окна с кнопкой "Open in-file..."
    connect(this->get_inFileLineEditor(), SIGNAL(textChanged(QString)), this, SLOT(openFileForEditing(QString)));	//связывает изменение в строке с in-file-ом с открытием программы в текстовом редакторе

    setLayout(mL_);
}

bool SimpleArtificialShell::checkExistenceAndCreateIfNot(const QString &filename) const{
	ifstream fin;
	fin.open(filename.toStdString().c_str(), ios::in);
	if (!fin.is_open()){
		//не можем открыть файл, т.к. его нет => создаем пустой файл.
		fin.close();

		ofstream fout;
		fout.open(filename.toStdString().c_str(), ios::out);
		if (!fout.is_open()){			//не можем создать файл, т.к., скорее всего, нет либо прав доступа, либо нет папки, в которой файл должен лежать.
			fttw::print_mistakeLite(fttw::toStr(__FUNCTION__)+"(const QString&)", __LINE__, __FILE__, "Can\'t create file with name:\n" + filename.toStdString() + "\nmay be you forgot to create folders?\n");
			QMessageBox::warning(0, tr("Attention"), tr("Can\'t create file with name:\n" + filename + "\nmay be you forgot to create folders or you have no permission to create file in this folder?"));
			return false;
		}

		fout.close();
	}
	else{
		fin.close();
	}
	return true;
}


void SimpleArtificialShell::chooseFileForEditing(){
	QString str = QFileDialog::getSaveFileName(0, "Choose in-file");
	if (checkExistenceAndCreateIfNot(str))
		inFileLab_->setText(str);
}


void SimpleArtificialShell::openFileForEditing(QString str){
	if (fileToTab_ != nullptr){
		//удалить внутренний FileToTabWidget с помощью reload_slot.
		fileToTab_->set_loadName(str.toStdString());
		fileToTab_->set_saveName(str.toStdString());
		fileToTab_->reload_slot();
	}
	else{
		fileToTab_ = new fttw::OutSideWidget(str.toStdString());
		mL_->addWidget(fileToTab_);
	}
}


void SimpleArtificialShell::openFileForEditingWithLineEditor(const QString& str){
	if (str != inFileLab_->text()){
		if (checkExistenceAndCreateIfNot(str))
			inFileLab_->setText(str);
	}
}


