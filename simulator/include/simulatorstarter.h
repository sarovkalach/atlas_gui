/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef SIMULATORSTARTER_H
#define SIMULATORSTARTER_H
#include <QProcess>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

#include <iostream>
#include <sstream>
using namespace std;

/*!
 *  \~russian \brief Класс для запуска симулятора на сервере.
 *
 * Класс может добавлять запись о запущенном процессе в БД
 * atlas.modelling, может запускать различные симуляторы на сервере.
 * Может отменять исполняемый процесс.
 *
 * Схема использования:
 * \code
 *
 * int owner = 4;
 * SimulatorStarter ss(owner);
 * QString infileName = "myInfileOnServer.smth";
 * QString simName = "Simulator CPU";
 * QString description = "Some description";
 *
 * ss.start(infileName, simName, description);
 *
 * //если хотим отменить исполнение программы, то
 * //ss.cancel();
 *
 * \endcode
 *
 */
 
class SimulatorStarter : public QObject{
	Q_OBJECT
private:
	// ------------------ Внешние переменные

	//! ID пользователя. Устанавливается в конструкторе.
	const int& owner_;

	// ------------------- Свои параметры запуска

	//! Имя папки, которая будет создана на локальном компьютере в директории с программой.
	QString sshfsDirName_		{"Server"};
	//! Имя сервера, с которым мы соединяемся.
	QString serverName_			{"atlas3d@85.143.2.188"};
	//! Имя директории на сервере, в которой будет создана отдельная папка для данного пользователя (или нет, если она уже существует). Будет выдаваться как директория по умолчанию при нажатии на кнопку "Open in-file" в случае, если удалось установить SSHFS соединение.
	QString dirOnServer_		{"atlas3d/simData"};
	//! Имя директории, где находятся исполняемые файлы симуляторов.
	QString simulatorsFolder_	{"/home/atlas3d/atlas3d/simulators/"};
	//! Путь к домашней папке на сервере. Нужен, чтобы понять, где на сервере пользователь создал папку.
	QString serverRoot_			{"/home/atlas3d"};
	//! Путь к bash-скрипту, который проверяет, если ли файл на сервере.
	QString fileChecker_		{"./fileChecker.sh"};

	// ------------------- Вспомогательные поля

	//! Вспомогательный объект для выполнения системных команд.
	QProcess executor_;
	//! Флаг. true, если соединение по sshfs получилось установить.
	bool sshfsSuccseed_ {false};
	//! Полное имя директории на сервере (относительно сервера), где будут храниться out, beg файлы.
	QString catalogOnServerName_;
	//! Основное имя, к которому уже добавляются расширения для соответствующих файлов.
	QString mainFilename_;
	//! Имя входного (in) файла (не полное).
	QString inFileName_;
	//! Идентификационный номер запущенного процесса. Берется после вставки строки о процессе в БД.
	QString processID_;
	//! Имя пользователя. Берется из таблицы atlas.users . Необходимо, чтобы создать папку с именем пользователя на сервере.
	QString userName_;

	//! Функция исполняет команду command с помощью executor_ . Ждет исполнения команды waitMS миллисекунд. Если программу удалось выполнить за отведенное время, то возвращает true. Если нет, то возвращает false, причем если установлен флаг closeProgramm, то закрывает текущую программу, а если установлен флаг messageBox, то показывает диалоговое окно с сообщением об ошибке (или предупреждении). \details Изменяет приватное поле executor_.
	bool executeCommand(const QString& command, int waitMS = 3000, bool messageBox = false, bool closeProgramm = false);

	//! Функция создает локальную папку и пытается соединить ее с сервером. Если получилось, то создает на сервере папку для пользователя - записывает true в sshfsSuccseed_. Также берет имя пользователя из БД atlas.users . \details Изменяет приватные поля executor_, sshfsSuccseed_, userName_.
	void tryToSSHFStoServer();
	//! Функция возвращает имя пользователя из БД, обращаясь к таблице atlas.users .
	QString getUserName() const;
	//! Функция ищет указанный пользователем входной файл на сервере. Если не находит, то возвращает false и выдает диалоговое окно с предупреждением. \details Изменяет приватные поля catalogOnServerName_, executor_ .
	bool findCatalogOnServer(const string& path, const string& file);
	//! Функция определяет полное имя до каталога относительно сервера, где будут храниться out, beg файлы, также определяет mainFilename_. \details Изменяет приватные поля catalogOnServerName_, mainFilename_, executor_ .
	bool defineCatalogOnServerName_and_MainFilename(const QString& choosedInFile);

	//!	По полученным данным вставляет строку в таблицу atlas.modelling в БД.
	bool insertRow(const QString& simName, const QString& description);

	//! Получает идентификационный номер симулятора по его имени.
	int getSimulatorID(const QString& simName) const;
	//! Функция возвращает идентификационный номер slurm_id по идентификационному номеру запущенного процесса.
	int getSlurmID() const;

public:
	//! Конструктор с начальными параметрами.
	explicit SimulatorStarter(const int& owner, QObject* pwg = Q_NULLPTR) : QObject(pwg),  owner_(owner) {}
	//! Деструктор.
	~SimulatorStarter() {}

	//! Функция инициализации. Должна быть вызвана лишь один раз внешним виджетом. Возвращает папку, которую следует открыть перед пользователем при нажатии на кнопку "Open in-file".
	QString init();

	//! Функция возвращает ID пользователя. \details Читает приватное поле owner_.
	const int& getOwner() const { return owner_; }
	//! Функция возвращает идентификационный номер запущенного процесса. \details Читает приватное поле processID_.
	const QString& getProcessID() const { return processID_; }

	//! Функция устанавливает имя папки, которая будет создана на локальном компьютере в директории с программой. \details Изменяет приватное поле sshfsDirName_ .
	void setSSHFSdirName(const QString& dirName) { sshfsDirName_ = dirName; }
	//! Функция возвращает имя папки, которая будет создана на локальном компьютере в директории с программой. \details Читает приватное поле sshfsDirName_ .
	const QString& getSSHFSdirName() const { return sshfsDirName_; }
	//! Функция устанавливает имя сервера. \details Изменяет приватное поле serverName_ .
	void setServerName(const QString& serverName) { serverName_ = serverName; }
	//! Функция возвращает имя сервера. \details Читает приватное поле serverName_ .
	const QString& getServerName() const { return serverName_; }
	//! Функция устанавливает имя директории на сервере, в которой будет создана отдельная папка для данного пользователя. \details Изменяет приватное поле dirOnServer_ .
	void setDirOnServerShortName(const QString& dirName) { dirOnServer_ = dirName; }
	//! Функция читает имя директории на сервере, в которой будет создана отдельная папка для данного пользователя. \details Читает приватное поле dirOnServer_ .
	const QString& getDirOnServerShortName() const { return dirOnServer_; }

	//! Функция возвращает полное имя директории на сервере (относительно сервера), где будут храниться out, in файлы. \details Читает приватное поле catalogOnServerName_ .
	const QString& getCatalogOnServerName() const { return catalogOnServerName_; }
	//! Функция возвращает основное имя, к которому уже добавляются расширения для соответствующих файлов. \details Читает приватное поле mainFilename_ .
	const QString& getMainFilename() const { return mainFilename_; }

	//! Добавляет запись о запуске симулятора в БД. Если успешно, то запускает симулятор на сервере.
	bool startSimulator(const QString& infileName, const QString &simName, const QString& description);
	//! Отменяет выполняющийся процесс. Удаляет его также из очереди на сервере.
	void cancel();

};

#endif // SIMULATORSTARTER_H

/*@}*/
