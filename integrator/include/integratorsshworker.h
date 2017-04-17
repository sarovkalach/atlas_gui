#ifndef INTEGRATORSSHWORKER_GASPARYANMOSES11042017
#define INTEGRATORSSHWORKER_GASPARYANMOSES11042017

#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QCryptographicHash>

#include <iostream>
#include <sstream>
#include <string>

#include "tableintegrelem.h"
using namespace std;


/*! \brief Класс для работы с БД по запуску конвертера.
 *
 * Позволяет установить SSHFS соединение с сервером через директорию.
 * Позволяет запустить конвертер на сервере. При запуске также добавляет
 * запись о нем вместе со всей дополнительной информацией.
 * Запись производится в таблицу atlas.integrate .
 *
 * Перед вызовом метода старт необходимо вызвать метод init().
 *
 * Схема использования:
 * \code
 *
 * int owner = 4;
 * TableIntegrElem elem;
 * //задать все поля структуры TableIntegrElem, означающие тип конвертера.
 * //...
 *
 * IntegratorSSHWorker puskach(owner);
 * QString dirName = puskach.init();
 *
 * QString path = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose dir", dirName);
 * puskach.start(path, elem);
 *
 * \endcode
 *
 */
class IntegratorSSHWorker final : public QObject{
	Q_OBJECT
private:
	//! Идентификационный номер
	const int& owner_;
	//! Имя директории, к которой будет подсоединен сервер с помощью sshfs.
	QString sshfsDirName_ {"Server"};
	//! Директория по умолчанию, в которой затем создаются, если необходимо, папки для каждого из пользователей в отдельности. Имя относительно корня сервера.
	QString deFaultDirWithDataOnServer_{"atlas3d/simData"};
	//! Имя сервера, к которому подключаемся.
	QString hostName_{"atlas3d@85.143.2.188"};
	//! Путь к скрипту, который проверяет, является ли поданный ему аргумент существующей директорией на сервере.
	QString dirCheker_{"./dirChecker.sh"};
	//! Путь к домашней папке сервера (куда мы попадаем после команды 'ssh <hostName>').
	QString serverRoot_{"/home/atlas3d"};

	//! Вспомогательный объект для исполнения различных системных команд.
	QProcess executor_;
	//! Каталог, где хранятся все входные и выходные файлы для запуска конвертера. Полное имя относительно сервера.
	QString catalogOnServerName_;
	//! Уникальный номер записи в базе данных. Выдается после того как запись о запуске была добавлена в БД (таблица atlas.integrate).
	int rowId_ {0};
	//! Лист полных имен входных файлов.
	QStringList inFiles_;
	//! Комментарий к запуску. Добавляется в запись к БД (таблица atlas.integrate).
	QString comment_;

	//! Функция берет имя пользователя из базы данных из таблицы atlas.users по идентификационному номеру пользователя owner_.
	QString getUserName() const;
	//! Функция исполняет команду и ожидает ее завершения waitMS миллисекунд. Если программа за это время не успела завершиться, то убивает процесс и возвращает false. Также может выдать окно с предупреждением об ошибке (регулируется флагом messageBox). Может также завершить исполнение программы с помощью выбрасывания исключения (регулируется флагом closeProgramm). \details Изменяет приватное поле executor_.
	bool executeCommand(const QString &command, int waitMS, bool messageBox, bool closeProgramm);
	//! Пытается подключиться по SSHFS к домашней папке сервера hostName_. Создает для этого отдельную папку sshfsDirName_ . Также создает папку для данного пользователя на сервере. Эта папка откроется по умолчанию при выборе входных файлов. Если все команды успешно исполнены, то возвращает true.
	bool tryToSSHFS();

	//! Функция смотрит, есть ли каталог path на сервере, заполняет поле catalogOnServerName_. path - имя каталога относительно локального компьютера. catalogOnServerName_ - имя каталога относительно сервера. Если каталог на сервере не найден, то возвращает false. \details Изменяет приватное поле catalogOnServerName_.
	bool findCatalogOnServer(const QString& path);

	//! Функция делает имя выходного файла из аргумента. \details Если у файла имеется расширение, то заменяет его на ".iout" . Если расширения нет, то добавляет к имени аргумента ".iout" .
	QString createOutFileNameFromOne(const QString& file) const;
	//! Функция делает имя выходного файла из аргумента и листа полных имен входных файлов. \details Если среди входных файлов имеется файл с расширением ".in" или ".iin", то берет его за основу и создает имя выходного файла из данного имени + ".iout" вместо расширения. Если входной файл только один, то вызывается функция createOutFileNameFromOne(). Если среди входных файлов нет файла с расширением ".in" или ".iin", то имя выходного файла строится следующим образом: "<имя пользователя>_<дата>_<хеш-функция от аргумента>.iout" .
	QString createOutFileName(const QString& allfiles) const;
	//! Функция вставляет строку в базу данных в таблицу atlas.integrate , используя информацию об интеграторе из аргумента. \details Изменяет приватное поле rowId_.
	bool insertRow(const TableIntegrElem& elem);

public:
	//! Конструктор. Принимает идентификационный номер пользователя.
	explicit IntegratorSSHWorker(const int& owner) : owner_(owner) {}
	//! Деструктор.
	~IntegratorSSHWorker() {}

	//! Функция инициализации. Должна быть вызвана до того, как использовать метод start() . Возвращает имя директории, которая должна быть открыта по умолчанию перед пользователем, когда он захотел выбрать входные файлы для конвертера. \details Изменяет приватное поле executor_.
	QString init();

	//! Функция устанавливает имя директории, к которой будет подсоединен сервер с помощью sshfs. \details Изменяет приватное поле sshfsDirName_.
	void setSSHFSdirName(const QString& dirName) { sshfsDirName_ = dirName; }
	//! Функция возвращает имя директории, к которой будет подсоединен сервер с помощью sshfs. \details Читает приватное поле sshfsDirName_.
	const QString& getSSHFSdirName() const { return sshfsDirName_; }
	//! Функция устанавливает директорию по умолчанию (относительное имя). \details Изменяет приватное поле deFaultDirWithDataOnServer_.
	void setDeFaultDirWithDataOnServer(const QString& dirName) { deFaultDirWithDataOnServer_ = dirName; }
	//! Функция возвращает имя директории по умолчанию (относительное). \details Читает приватное поле deFaultDirWithDataOnServer_.
	const QString& getDeFaultDirWithDataOnServer() const { return deFaultDirWithDataOnServer_; }
	//! Функция устанавливает имя сервера, к которому мы подключаемся. \details Изменяет приватное поле hostName_.
	void setHostName(const QString& hostName) { hostName_ = hostName; }
	//! Функция возвращает имя сервера, к которому мы подключаемся. \details Читает приватное поле hostName_.
	const QString& getHostName() const { return hostName_; }
	//! Функция устанавливает путь до скрипта, который проверяет, является ли поданный ему аргумент существующей директорией на сервере. \details Изменяет приватное поле dirCheker_.
	void setDirChecker(const QString& dirChecker) { dirCheker_ = dirChecker; }
	//! Функция возвращает путь до скрипта, который проверяет, является ли поданный ему аргумент существующей директорией на сервере. \details Читает приватное поле dirCheker_.
	const QString& getDirChecker() const { return dirCheker_; }
	//! Функция устанавливает путь к домашней папке сервера. \details Изменяет приватное поле serverRoot_.
	void setServerRoot(const QString& servRoot) { serverRoot_ = servRoot; }
	//! Функция возвращает путь к домашней папке сервера. \details Читает приватное поле serverRoot_.
	const QString& getServerRoot() const { return serverRoot_; }

	//! Функция возвращает имя каталога, где хранятся все входные и все выходные файлы конвертера (полный путь относительно сервера). \details Читает приватное поле catalogOnServerName_.
	const QString& getCatalogOnServerName() const { return catalogOnServerName_; }
	//! Функция возвращает уникальный номер записи о запуске конвертера в БД (из таблицы atlas.integrate). \details Читает приватное поле rowId_.
	const int& getRowID() const { return rowId_; }
	//! Функция устанавливает лист полных имен входных файлов. \details Изменяет приватное поле inFiles_.
	void setInFiles(const QStringList& sl) { inFiles_ = sl; }
	//! Функция возвращает лист полных имен входных файлов. \details Читает приватное поле inFiles_.
	const QStringList& getInFiles() const { return inFiles_; }
	//! Функция устанавливает комментарий к запуску. \details Изменяет приватное поле comment_.
	void setComment(const QString& s) { comment_ = s; }
	//! Функция возвращает комментарий к запуску. \details Читает приватное поле comment_.
	const QString& getComment() const { return comment_; }

	//! Функция запускает конвертер на сервере на исполнение, передавая ему в качестве аргумента уникальный номер записи о запуске в базе данных таблицы atlas.integrate .
	bool start(const QString& path, const TableIntegrElem& elem);

};

#endif // GASPARYANMOSES11042017
