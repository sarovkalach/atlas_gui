#include "integratorsshworker.h"

bool IntegratorSSHWorker::executeCommand(const QString &command, int waitMS, bool messageBox, bool closeProgramm){
	executor_.start(command);
	bool finished = executor_.waitForFinished(waitMS);
	if (!finished){
		if (closeProgramm){
			cout<<"ERROR ";
		}
		else{
			cout<<"WARNING ";
		}
		cout<<"in IntegratorSSHWorker::executeCommand(const QString &command, int waitMS, bool messageBox, bool closeProgramm) : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tcan\'t execute command \""<<command.toStdString()<<"\""<<endl;
		if (messageBox){
			if (closeProgramm){
				QMessageBox::critical(Q_NULLPTR, tr("Command error"), tr("Can\'t execute command \"") + command + "\"");
				throw "QProcess command error";
			}
			else{
				QMessageBox::warning(Q_NULLPTR, tr("Command error"), tr("Can\'t execute command \"") + command + "\"");
			}
		}
		executor_.kill();
	}
	return finished;
}

QString IntegratorSSHWorker::getUserName() const{
	QSqlQuery query;
	QSqlQueryModel queryModel;

	query.prepare("SELECT login FROM atlas.users WHERE id=:ref");
	query.bindValue(":ref", owner_);
	if (!query.exec()){
		QMessageBox::critical(Q_NULLPTR, tr("Error"), tr("Sorry, can\'t receive your userName from Database."));
		cout<<"ERROR in IntegratorSSHWorker::getUserName() const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tmay be it connected with bad setting of ownerID in this class or connection with Database suddenly was corrupted by evil force..."<<endl;
		cout<<"\townerID = "<<owner_<<endl;
		throw "Can\'t receive userName from Database";
	}

	queryModel.setQuery(query);
	return queryModel.data(queryModel.index(0,0)).toString();
}

bool IntegratorSSHWorker::tryToSSHFS(){
	QString command;

	command = "mkdir "+sshfsDirName_;
	qDebug()<<"using command: "<<command;
	if (!executeCommand(command, 3000, 1, 0)) {return false;}

	command = "sshfs "+sshfsDirName_+" "+hostName_+":";
	qDebug()<<"using command: "<<command;
	if (!executeCommand(command, 5000, 1, 0)) {return false;}

	QString userName = getUserName();
	qDebug()<<"userName = "<<userName;
	command = "mkdir " + sshfsDirName_+ "/" +deFaultDirWithDataOnServer_ + "/" + userName;
	qDebug()<<"using command: "<<command;
	if (!executeCommand(command, 5000, 1, 0)) {return false;}
	return true;
}

bool IntegratorSSHWorker::findCatalogOnServer(const QString& path){
	qDebug()<<"FROM findCatalogOnServer";
	qDebug()<<"path_in = "<<path;
	string userPath = path.toStdString();
	if (path[path.size()-1] == '/')
		userPath = path.toStdString().substr(0,path.size()-1);
	else
		userPath = path.toStdString();


	cout<<"userPath = "<<userPath<<endl;

	bool dirOnServer = true;

	QString command;
	stringstream vss;
	int resInt = -1;
	while(1){
		command = dirCheker_ + " " + serverRoot_ + "/" + QString::fromStdString(userPath);
		qDebug()<<"command = "<<command;
		executeCommand(command, 5000, 0,0);
		string res = executor_.readAllStandardOutput().toStdString();
		cout<<"\tout = "<<res<<endl;
		resInt = -1;
		vss.str(res);
		vss>>resInt;
		if (resInt == 0) {break;}		//директория существует
		else{
			size_t found = userPath.find_first_of("/");
			if (found < userPath.size())
				userPath = userPath.substr( found+1 , userPath.size() );
			else
				userPath = "";
		}

		if (userPath.empty()){
			dirOnServer = false;
			break;
		}
		vss.clear();
		vss.str("");
	}

	if (!dirOnServer){
		QMessageBox::warning(Q_NULLPTR, tr("Attention"), tr("Your dir is not on Server. Please, choose another dir which belongs to the server folder (use SSHFS)."));
		return false;
	}
	catalogOnServerName_ = serverRoot_ + "/" + QString::fromStdString(userPath);
	return true;
}

QString IntegratorSSHWorker::createOutFileNameFromOne(const QString &file) const{
	string res;
	string str = file.toStdString();
	size_t found = str.find_last_of('.');
	if (found != 0){
		res = str.substr(0, found) + ".iout";
		if (str == res)
			res += ".iout";
		return QString::fromStdString(res);
	}
	else
		return QString::fromStdString(str) + ".iout";
}

QString IntegratorSSHWorker::createOutFileName(const QString& allfiles) const{
	if (inFiles_.size() == 1){
		return createOutFileNameFromOne(allfiles);
	}
	else if (inFiles_.size() > 0){
		QString nameWith_In;
		typename QStringList::const_iterator it = inFiles_.cbegin();
		size_t found;
		string vs;
		string substr;
		for(; it != inFiles_.cend(); ++it){
			vs = it->toStdString();
			found = vs.find_last_of('.');
			substr = vs.substr(found);
			if ( (found != 0) &&
			     (substr == ".in" || substr == ".iin") ){
				nameWith_In = *it;
				break;
			}
		}

		if (nameWith_In.isEmpty()){
			QString hashh;
			hashh = QString( QCryptographicHash::hash(allfiles.toStdString().c_str(), QCryptographicHash::Md5).toHex() );
			return getUserName() + "_" + QDateTime().currentDateTime().toString("dd.mm.yyyy") + "_" + hashh + ".iout";
		}
		else
			return createOutFileNameFromOne(nameWith_In);
	}
	else{
		return getUserName() + ".iout";
	}
}

bool IntegratorSSHWorker::insertRow(const TableIntegrElem& elem){
	QSqlQuery query;
	query.prepare("INSERT INTO atlas.integrate (owner,date,in_file,out_file,integrator_id,comments,catalog) VALUES (:owner, :date, :in_file, :out_file, :integrator_id, :comments, :catalog)");
	query.bindValue(":owner", owner_);
	query.bindValue(":date", QDateTime().currentDateTime().toString("MM.dd.yyyy"));

	QString allfiles;
	typename QStringList::const_iterator it = inFiles_.cbegin();
	if (inFiles_.size() > 0){
		allfiles = *it;
		++it;
		for(; it != inFiles_.cend(); ++it){
			allfiles += " ; " + *it;
		}
		qDebug()<<"allInFiles = "<<allfiles;
	}
	query.bindValue(":in_file", allfiles);

	QString outFileName = createOutFileName(allfiles);
	qDebug()<<"outFileName = "<<outFileName;
	query.bindValue(":out_file", outFileName);

	query.bindValue(":integrator_id", elem.id);
	query.bindValue(":comments", comment_);
	query.bindValue(":catalog", catalogOnServerName_);

	if (query.exec()){
		rowId_ = query.lastInsertId().toInt();
		return true;
	}
	else{
		QMessageBox::warning(Q_NULLPTR, tr("SQL Database error"), tr("Can\'t execute query for inserting row in Database. May be your connection with Database has been broken?"));
		rowId_ = 0;
		return false;
	}
}

bool IntegratorSSHWorker::start(const QString& path, const TableIntegrElem& elem){
	qDebug()<<"FROM IntegratorSSHWorker::start :";
	qDebug()<<"path = "<<path;

	if (!findCatalogOnServer(path)) {return false;}
	if (!insertRow(elem)) {return false;}

	stringstream vss;
	vss<<rowId_;
	QString command;
	command = "ssh " + hostName_ + " "+ elem.ref + " " + QString::fromStdString(vss.str()) + "";
	qDebug()<<"executing command = "<<command;
	return executeCommand(command, 5000, 1, 0);
}

QString IntegratorSSHWorker::init(){
	if (tryToSSHFS())
		return sshfsDirName_+ "/" +deFaultDirWithDataOnServer_ + "/" + getUserName();
	else
		return "";
}
