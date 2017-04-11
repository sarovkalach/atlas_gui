/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef GENERALWINDOW_03042017_GASPARYANMOSES
#define GENERALWINDOW_03042017_GASPARYANMOSES

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include "userrecord.h"

/*! \brief Общее окно.
 *
 * Общее окно для интерфейса пользователя
 * глоссария, симулятора, интегратора.
 *
 * Включает в себя основные компоненты меню:
 * вызов окна авторизации, меню помощи.
 *
 * Может быть создано только наследниками.
 *
 * Схема использования:
 * \code
 *
 * \endcode
 *
 */
class GeneralWindow : public QMainWindow{
	Q_OBJECT
private:
	//! Функция для инициализации. Должна быть вызвана только один раз в конструкторе.
	void init();

protected:
	//! Объект для выдачи окна авторизации, а также проверки логина и пароля по файлу.
	UserRecord userRecord_{"enter_file.txt"};
	//! Меню, связанное с аккаунтом.
	QMenu* menuAccount;
	//! Меню, связанное с помощью.
	QMenu* menuHelp;

	//! Идентификационный номер владельца. Задается через окно авторизации.
	int owner_;

	//! Конструктор. Выдает диалоговое окно авторизации.
	explicit GeneralWindow(QWidget* pwgt = Q_NULLPTR) : QMainWindow(pwgt) {init();}
	//! Конструктор с указанием имени файла для сохранения данных о пользователе. Выдает диалоговое окно авторизации.
	explicit GeneralWindow(const QString& enterFileName, QWidget* pwgt = Q_NULLPTR) : QMainWindow(pwgt), userRecord_(enterFileName) {init();}

	//! Деструктор.
	~GeneralWindow() {}

	virtual void loginUser(bool checkEnterFile) = 0;

protected slots:
	//! Метод для открытия диалогового окна авторизации.
	void loginUserSlot(bool checkEnterFile = false);

};

#endif // GENERALWINDOW_03042017_GASPARYANMOSES

/*@}*/
