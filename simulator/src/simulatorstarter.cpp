#include "simulatorstarter.h"

QString SimulatorStarter::getUserName() const{
	QSqlQuery query;
	QSqlQueryModel queryModel;

	query.prepare("SELECT login FROM atlas.users WHERE id=:ref");
	query.bindValue(":ref", owner_);

	bool success = query.exec();
	if (!success){
		QMessageBox::critical(Q_NULLPTR, tr("Error"), tr("Sorry, can\'t receive your userName from Database."));
		cout<<"ERROR in SimulatorStarter::getUserName() const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tmay be it connected with bad setting of ownerID in this class or connection with Database suddenly was corrupted by evil force..."<<endl;
		cout<<"\townerID = "<<owner_<<endl;
		throw "Can\'t receive userName from Database";
	}

	queryModel.setQuery(query);
	return queryModel.data(queryModel.index(0,0)).toString();
}

bool SimulatorStarter::executeCommand(const QString &command, int waitMS, bool messageBox, bool closeProgramm){
	executor_.start(command);
	bool finished = executor_.waitForFinished(waitMS);
	if (!finished){
		if (closeProgramm){
			cout<<"ERROR ";
		}
		else{
			cout<<"WARNING ";
		}
		cout<<"in SimulatorStarter::executeCommand(const QString &command, int waitMS, bool messageBox, bool closeProgramm) : line "<<__LINE__<<", file "<<__FILE__<<endl;
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
	}
	return finished;
}

void SimulatorStarter::tryToSSHFStoServer(){
	QString command = "mkdir " + sshfsDirName_;
	executeCommand(command);

	command = "sshfs " + sshfsDirName_ + " " + serverName_ + ":";
	sshfsSuccseed_ = executeCommand(command);

	if (sshfsSuccseed_){
		userName_ = getUserName();
		command = "ssh " + serverName_ + " mkdir " + "./" + dirOnServer_ + "/" + userName_;
		sshfsSuccseed_ = executeCommand(command);
	}
}

bool SimulatorStarter::findCatalogOnServer(const string& path, const string& file){
	string realServerPath = path.substr(1, path.size());
	bool fileOnServer = true;
	stringstream vss;
	QString command;
	qDebug()<<"FINDING CATALOG ON SERVER";
	while(1){
		command = fileChecker_ + " " + serverRoot_ + "/" + QString::fromStdString(realServerPath + file);
		qDebug()<<"command = "<<command;
		executeCommand(command);
		string res = executor_.readAllStandardOutput().toStdString();
		int resInt = -1;
		vss.str(res);
		vss>>resInt;
		if (resInt == 0){				//файл существует
			break;
		}
		else{
			realServerPath = realServerPath.substr( realServerPath.find_first_of("/")+1 , realServerPath.size());
		}

		if (realServerPath.empty()){
			fileOnServer = false;
			break;
		}
		vss.clear();
		vss.str("");
	}

	if (!fileOnServer){
		QMessageBox::warning(Q_NULLPTR, tr("Attention"), tr("Your in-file is not on Server. Please, choose another in-file which belongs to the server folder (use SSHFS)."));
		return false;
	}
	catalogOnServerName_ = serverRoot_ + "/" + QString::fromStdString(realServerPath);		// "/home/atlas3d/atlas3d/simData/"

	return true;
}

bool SimulatorStarter::defineCatalogOnServerName_and_MainFilename(const QString& inFileName){
	string choosedInFile = inFileName.toStdString();
	size_t found = choosedInFile.find_last_of("/\\");

	string path = choosedInFile.substr(0, found+1);		//путь файла в стиле "/home/moses/"
	string file = choosedInFile.substr(found+1);		//файл в стиле "init.txt"
	inFileName_ = QString::fromStdString(file);

	found = file.find_last_of('.');
	if (found != 0)
		mainFilename_ = QString::fromStdString(file.substr(0, found));				//уберем точку из разрешения файла "init"
	else
		mainFilename_ = QString::fromStdString(file);								//если имя файла начинается с точки и не содержит затем точек ".bashrc"

	//
	cout<<"From Simulator Starter:"<<endl;
	cout<<"path = \""<<path<<"\""<<endl;
	cout<<"file = \""<<file<<"\""<<endl;
	cout<<"mainFilename_ = \""<<mainFilename_.toStdString()<<endl<<endl;
	//
	return findCatalogOnServer(path, file);
}


QString SimulatorStarter::init(){
	tryToSSHFStoServer();
	if (sshfsSuccseed_){
		qDebug()<<"defaultDir = "<<"./" + sshfsDirName_+ "/" + dirOnServer_ + "/" + userName_;
		return "./" + sshfsDirName_+ "/" + dirOnServer_ + "/" + userName_;
	}
	return "";
}

int SimulatorStarter::getSimulatorID(const QString &simName) const{
	QSqlQuery query;
	QSqlQueryModel queryModel;
	query.prepare("SELECT id FROM atlas.simulator WHERE reference=:ref");
	query.bindValue(":ref", simName);
	if (!query.exec()){
		QMessageBox::warning(Q_NULLPTR, tr("Wrong simulator name"), tr("Sorry, can\'t find simulator ID from Database with name \"") + simName + "\"");
		return -1;
	}
	queryModel.setQuery(query);
	return queryModel.data(queryModel.index(0,0)).toInt();
}

bool SimulatorStarter::insertRow(const QString& simName, const QString& description){
	QSqlQuery query;

	query.prepare("INSERT INTO atlas.modelling (owner, beg_date, simulator_id, in_file, out_file, description, progress, slurm_id, catalog) VALUES (:owner, :beg_date, :simulator_id, :in_file, :out_file, :description, :progress, :slurm_id, :catalog)");

	query.bindValue(":owner", owner_);
	query.bindValue(":beg_date", QDateTime().currentDateTime().toString("MM.dd.yyyy"));

	int simID = getSimulatorID(simName);
	if (simID < 0){return false;}
	query.bindValue(":simulator_id", simID);

	query.bindValue(":in_file", inFileName_);

	QString endFilename = mainFilename_ + ".out";
	if (endFilename == inFileName_) {endFilename += ".out";}
	query.bindValue(":out_file", endFilename);

	query.bindValue(":description", description);
	query.bindValue(":progress", 0);
	query.bindValue(":catalog", catalogOnServerName_);

	if (!query.exec()){
		qDebug()<<"Can\'t insert row in Database";
		QMessageBox::warning(Q_NULLPTR, tr("Can\'t exec the query"), tr("Can\'t insert row in database"));
		return false;
	}
	qDebug()<<"Row was successfully inserted in Database";
	processID_ = query.lastInsertId().toString();
	return true;
}

bool SimulatorStarter::startSimulator(const QString& infileName, const QString &simName, const QString& description){
	qDebug()<<"defining catalog on server";
	bool succeed = defineCatalogOnServerName_and_MainFilename(infileName);
	qDebug()<<"server path = "<<catalogOnServerName_;
	if (succeed){
		if (!insertRow(simName, description))
			return false;

		QString command = "ssh " + serverName_+ " " + simulatorsFolder_;
		if (simName == "Simulator CPU"){
			command += "startSimCPU.sh ";
		}
		else{
			command += "startSimGPU.sh ";
		}
		command += processID_;

		qDebug()<<"Start experiment with command = " + command;
		executor_.start(command);
		return true;
	}
	else{
		return false;
	}
}

int SimulatorStarter::getSlurmID() const{
	QSqlQuery query;
	QSqlQueryModel queryModel;

	query.prepare("SELECT slurm_id FROM atlas.modelling WHERE id=:ref1");
	query.bindValue(":ref1", processID_);
	query.exec();
	queryModel.setQuery(query);
	if (!(queryModel.index(0,0).isValid())){
		QMessageBox::Button res = QMessageBox::warning(Q_NULLPTR, tr("Warning"), tr("Can\'t stop process, because slurmID was not received yet.\nPlease, wait. This task usually takes several seconds.\n\nDo you want to stop process in hard way?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (res == QMessageBox::Yes)
			return -1;
		else
			return 0;
	}
	return queryModel.data(queryModel.index(0,0)).toInt();
}

void SimulatorStarter::cancel(){
	int slurmID = getSlurmID();

	if (slurmID == 0){
		return;
	}
	qDebug() << "Stop Experiment";
	executor_.close();

	if (slurmID > 0){
		stringstream vss;
		vss<<slurmID;
		executor_.start("ssh " + serverName_ + " scancel " + QString::fromStdString(vss.str()));
	}
}
