#include "../general_gui_files/include/userrecord.h"

bool LoginDialog::checkUserIDfromDB(const QString& login, const QString& pass, int& owner){
	QString hash = QString( QCryptographicHash::hash((login + pass + "ILoveMassEffect").toStdString().c_str(), QCryptographicHash::Md5).toHex() );
	return checkUserIDfromDB(login, pass, hash, owner);
}

bool LoginDialog::checkUserIDfromDB(const QString &login, const QString &pass, const QString& hashh, int &owner){
	QSqlQuery query;
	query.prepare("SELECT * FROM atlas.users WHERE login = :login");
	query.bindValue(":login", login);
	query.exec();

	if (query.first()){
		QString passFromDB, hashFromDB;
		owner = query.value(0).toInt();
		passFromDB = query.value(2).toString();

		hashFromDB = QString( QCryptographicHash::hash((login + passFromDB + "ILoveMassEffect").toStdString().c_str(), QCryptographicHash::Md5).toHex() );

		if ( (hashh == hashFromDB) && ( pass == passFromDB ) )
			return true;
		else
			return false;
	}
	else
		return false;
}

// --------------------- LoginDialog

void LoginDialog::sendLoginAndPass(){
	login_ = loginE_->text();
	pass_ = passE_->text();

	if (checkUserIDfromDB(login_, pass_, owner_)){
		accept();
	}
	else{
	//	wrong_->show();
		wrong_->showMessage(tr("Wrong account"), 5000);
	}
}

LoginDialog::LoginDialog(bool& marked, QString& login, QString& pass, int& owner, QWidget* pwg) : QDialog(pwg), marked_(marked), login_(login), pass_(pass), owner_(owner){
	mL_ = new QVBoxLayout;
	buttonLayout_ = new QHBoxLayout;

	loginL_ = new QLabel(tr("User"));
	loginE_ = new QLineEdit;
	passL_ = new QLabel(tr("Password"));
	passE_ = new QLineEdit;
	passE_->setEchoMode(QLineEdit::EchoMode::Password);

	okButton_ = new QPushButton(tr("Ok"));
	cancelButton_ = new QPushButton(tr("Cancel"));

	saveSession_ = new QCheckBox(tr("Save session"));

//	wrong_ = new QLabel(tr("Wrong account"));
	wrong_ = new QStatusBar;
//	wrong_->hide();

	buttonLayout_->addWidget(okButton_);
	buttonLayout_->addWidget(cancelButton_);

	mL_->addWidget(loginL_);
	mL_->addWidget(loginE_);
	mL_->addWidget(passL_);
	mL_->addWidget(passE_);
	mL_->addLayout(buttonLayout_);
	mL_->addWidget(saveSession_);
	mL_->addWidget(wrong_);

	setLayout(mL_);
	setModal(true);

	connect(okButton_, SIGNAL(clicked()), this, SLOT(sendLoginAndPass()));
	connect(cancelButton_, SIGNAL(clicked()), this, SLOT(reject()));
	connect(saveSession_, SIGNAL(stateChanged(int)), this, SLOT(setMarked(int)));
}

// --------------------- UserRecord

void UserRecord::saveInFile(const QString& login, const QString& password, const QString& hash) const{
	ofstream fout;
	fout.open(filename_.toStdString().c_str(), ios::out);

	if (!fout.is_open()){
		QMessageBox::warning(0, "Wrong path",
		  "Can\'t create file with name\n\t" +
		  filename_  +
		  "\n\nMay be programm have no permission to create files in folder or there is no created folders in path above.",
		  QMessageBox::Ok);
		fout.close();
		return;
	}

	fout<<login.toStdString()<<endl;
	fout<<password.toStdString()<<endl;
	fout<<hash.toStdString()<<endl;

	fout.close();
}

bool UserRecord::checkWithHashedPass(const QString &login, const QString &pass, const QString &hashh, int &owner){
	QSqlQuery query;
	query.prepare("SELECT * FROM atlas.users WHERE login = :login");
	query.bindValue(":login", login);
	query.exec();

	if (query.first()){
		QString passFromDB, hashFromDB;
		owner = query.value(0).toInt();
		passFromDB = query.value(2).toString();

		hashFromDB = QString( QCryptographicHash::hash((login + passFromDB + "ILoveMassEffect").toStdString().c_str(), QCryptographicHash::Md5).toHex() );

		passFromDB = QString( QCryptographicHash::hash((passFromDB + "GarrusForever_TurianFleet...").toStdString().c_str(), QCryptographicHash::Md5).toHex() );

		if ( (hashh == hashFromDB) && ( pass == passFromDB ) )
			return true;
		else
			return false;
	}
	else
		return false;
}

int UserRecord::tryToGetOwner(bool checkFile){
	QString login, pass, hash;
	string loginStd, passStd, hashStd;
	int owner = -100;

	QString hashForPass = "GarrusForever_TurianFleet...";

	bool rightInFile = true;

	if (checkFile){			//если необходимо проверить файл.
		ifstream fin;
		fin.open(filename_.toStdString().c_str(), ios::in);
		if (fin.is_open()){
			fin>>loginStd>>passStd>>hashStd;
		}
		fin.close();

		login = QString::fromStdString(loginStd);
		pass = QString::fromStdString(passStd);
		hash = QString::fromStdString(hashStd);

		rightInFile = checkWithHashedPass(login, pass, hash, owner);

		if (!rightInFile){
			qDebug()<<"Wrong information in file";
			cout<<"In file:"<<endl;
			cout<<"login:\t"<<loginStd<<endl;
			cout<<"pass:\t"<<passStd<<endl;
			cout<<"hash:\t"<<hashStd<<endl<<endl;
		}
	}
	if ((!rightInFile) || (!checkFile) ){					//если нужно проверить без файла или в файле некорректные сведения
		bool mark = false;
		int res = LoginDialog::createLoginDialog(mark, login, pass, owner);

		hash = QString( QCryptographicHash::hash((login + pass + "ILoveMassEffect").toStdString().c_str(), QCryptographicHash::Md5).toHex() );
		saveInFile("", "", "");										//удалить всю информацию в файле.
		if (res == QDialog::Accepted){
			qDebug()<<"Access granted";
			if (mark){
				qDebug()<<"saving in file.............";
				pass = QString( QCryptographicHash::hash((pass + "GarrusForever_TurianFleet...").toStdString().c_str(), QCryptographicHash::Md5).toHex() );
				saveInFile(login, pass, hash);
			}
		}
		else{
			qDebug()<<"Canceled";
			throw QString("Application was cancelled");
		}
	}
	return owner;
}
