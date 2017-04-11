/** @addtogroup oscillators
 * @{*/

/** @file */

#ifndef USERRECORD_GASPARYANMOSES_29032017
#define USERRECORD_GASPARYANMOSES_29032017

#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QStatusBar>

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class UserRecord;
/*! \brief Окно авторизации.
 *
 * Окно авторизации,
 * которое проверяет, есть ли пользователь в БД.
 * Если пользователя нет, то показывает "Wrong account".
 *
 * При нажатии кнопки отмена или закрытии окна завершает программу.
 *
 * Конструктор принимает несколько параметров-ссылок, которые
 * при нажатии кнопки "Ок" заполняются - логин, пароль,
 * флаг надо ли сохранять в файле данные, ID владельца =>
 * они являются возвращаемыми значениями.
 *
 * Схема использования:
 * \code
 *
 *	bool mark;
 *	int owner;
 *	QString login, pass;
 *
 *	int res = LoginDialog::createLoginDialog(mark, login, pass, owner);
 *
 *	if (res == QDialog::Accepted){
 *		qDebug()<<"Access granted";
 *	}
 *	else{
 *		qDebug()<<"Canceled";
 *	}
 *
 * \endcode
 *
 */
class LoginDialog final : public QDialog{
	Q_OBJECT

private:
	//! Проверка логина и пароля. Вызывается, если неизвестен хэш. Функция возвращает true, если запись есть в базе данных. false, если логин или пароль некорректно введены. Изменяет аргумент owner.
	static bool checkUserIDfromDB(const QString& login, const QString& pass, int& owner);
	//! Проверка логина, пароля и хэша. Вызывается, если считывание произошло из файла. Функция возвращает true, если запись есть в базе данных. false, если логин или пароль некорректно введены. Изменяет аргумент owner.
	static bool checkUserIDfromDB(const QString& login, const QString& pass, const QString& hash, int& owner);

	friend UserRecord;
	//! Флаг, обозначающий, что нажата галочка для сохранения данных в файле (чтобы ещё раз не пришлось вписывать логин и пароль).
	bool& marked_;
	//! Логин.
	QString& login_;
	//! Пароль.
	QString& pass_;
	//! Идентификационный номер владельца.
	int& owner_;

	//! Главный менеджер размещения.
	QVBoxLayout* mL_;
	//! Надпись "User" или "Логин".
	QLabel* loginL_;
	//! Редактор, куда можно писать свой логин.
	QLineEdit* loginE_;
	//! Надпись "Password" или "Пароль".
	QLabel* passL_;
	//! Редактор, куда можно писать свой пароль. Включет мод для пароля.
	QLineEdit* passE_;

	//! Менеджер размещения для кнопок.
	QHBoxLayout* buttonLayout_;
	//! Кнопка "Ок".
	QPushButton* okButton_;
	//! Кнопка "Cancel"
	QPushButton* cancelButton_;

	//! Флаг - сохранить ли данные о пользователе в файле, чтобы затем заходить без запроса пароля и логина.
	QCheckBox* saveSession_;

	//! Строка состояния, которая показывает "Wrong account", если пароля или логина нет в БД.
	QStatusBar* wrong_;

	//! Конструктор. Принимает 4 параметра, которые являются результатом работы диалогового окна.
	LoginDialog(bool& marked, QString& login, QString& pass, int& owner, QWidget* pwg = Q_NULLPTR);
	//! Деструктор.
	~LoginDialog() {}

	//! Статическая функция, которая создает диалоговое окно и исполняет его.
	static int createLoginDialog(bool& marked, QString& login, QString& pass, int& owner, QWidget* pwg = Q_NULLPTR){
		LoginDialog ld(marked, login, pass, owner, pwg);
		return ld.exec();
	}

private slots:
	//! Слот изменяет флаг сохранения данных о пользователе в файле. \details Изменяет приватное поле marked_.
	void setMarked(int mark) { marked_ = mark; }
	//! Слот вызывается при нажатии на кнопку "Ок". Записывает всё в поля, которые являлись входными параметрами конструктора. Если же пользователя нет в БД, то отображает это в строке состояния и не завершает диалоговое окно. \details Изменяет приватные поля login_, pass_, owner_.
	void sendLoginAndPass();
};



/*! \brief Оболочка для окна авторизации.
 *
 * Сначала проверяет, существует ли файл filename_.
 * Если нет или в файле некорректные данные (не совпадающие с БД),
 * то выдает окно авторизации. Далее в зависимости от галочки,
 * поставленной пользователем, сохраняет данные о пользователе
 * или удаляет их.
 *
 * При отказе авторизовываться пробрасывает исключение.
 *
 * Схема использования:
 * \code
 *
 * UserRecord userRecord_("myfile223.txt");
 *
 * int owner_;
 * owner_ = userRecord_.tryToGetOwner(true);
 *
 * \endcode
 *
 */
class UserRecord final : public QObject{
	//! Имя файла, в который идет сохранение данных о пользвателе и из которого идет загрузка данных.
	QString filename_ {"myfile.txt"};

	//! Функция записывает данные о пользователе в файл filename_.
	void saveInFile(const QString& login, const QString& password, const QString& hash) const;

public:
	//! Пустой конструктор.
	explicit UserRecord(QObject* pwg = Q_NULLPTR) : QObject(pwg) {}
	//! Конструктор с указанием имени файла.
	explicit UserRecord(const QString& filename, QObject* pwg = Q_NULLPTR) : QObject(pwg), filename_(filename) {}
	//! Деструктор.
	~UserRecord() {}

	bool checkWithHashedPass(const QString& login, const QString& pass, const QString& hashh, int& owner);

	//! Функция устанавливает имя файла для сохранения данных о пользователе. \details Изменяет приватное поле filename_.
	void setFilename(const QString& name) { filename_ = name; }

	//! Функция открывает перед пользователем диалоговое окно, если входной параметр равен false или если в файле filename_ информация не соответствует БД. Далее в зависимости от галочки в диалоговом окне сохраняет данные о пользователе в файл или нет.
	int tryToGetOwner(bool checkFile);

};

#endif // USERRECORD_GASPARYANMOSES_29032017

/*@}*/
