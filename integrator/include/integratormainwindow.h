#ifndef INTEGRATORMAINWINDOW_GASPARYANMOSES_11042017
#define INTEGRATORMAINWINDOW_GASPARYANMOSES_11042017

#include "../general_gui_files/include/generalwindow.h"
#include "integratorwidget.h"

/*! \brief Главное окно интегратора.
 *
 * Класс наследуется от общего окна GeneralWindow.
 *
 * Содержит все необходимые действия, панель инструментов,
 * строку состояния, меню, относящиеся к интегратору.
 *
 * Также содержит главный виджет интегратора.
 * При своем создании выдает окно авторизации, если
 * сессия не была сохранена.
 *
 * Схема использования:
 * \code
 *
 * IntegratorMainWindow mW;
 * mW.show();
 *
 * \endcode
 *
 */
class IntegratorMainWindow : public GeneralWindow{
	Q_OBJECT
private:
	//! Главный виджет интегратора.
	IntegratorWidget* integratorWidget_;

protected:
	//! Метод для открытия диалогового окна авторизации.
	virtual void loginUser(bool checkEnterFile = false) override;

public:
	//! Конструктор.
	explicit IntegratorMainWindow(QWidget* pwg = Q_NULLPTR);
};

#endif // INTEGRATORMAINWINDOW_GASPARYANMOSES_11042017
