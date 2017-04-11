/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef SIMULATORMAINWINDOW_03042017_GASPARYANMOSES
#define SIMULATORMAINWINDOW_03042017_GASPARYANMOSES

#include "../general_gui_files/include/generalwindow.h"
#include "simulatorwidget.h"

/*! \brief Главное окно GUI симулятора.
 *
 * Наследник общего окна GeneralWindow.
 * Является главным окном для симулятора.
 * Содержит внутри себя виджет со всеми кнопками и виджетами для
 * симулятора, а также все необходимые меню,
 * панели инструментов (если необходимо).
 *
 * Схема использования:
 * \code
 *
 * 	SimulatorMainWindow* mw = new SimulatorMainWindow;
 *	mw->show();
 *
 *  delete SimulatorMainWindow;
 *
 * \endcode
 *
 */
class SimulatorMainWindow final : public GeneralWindow{
	Q_OBJECT
private:
	//! Виджет со всеми кнопками, другими виджетами, которые присущи симулятору. Является главным виджетом для данного окна.
	SimulatorWidget* simulatorWidget_;

protected:
	//! Метод для открытия диалогового окна авторизации.
	virtual void loginUser(bool checkEnterFile = false) override;

public:
	//! Конструктор.
	explicit SimulatorMainWindow(QWidget* pwgt = Q_NULLPTR);
	//! Деструктор.
	~SimulatorMainWindow() {}

};

#endif // SIMULATORMAINWINDOW_03042017_GASPARYANMOSES

/*@}*/
