/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef SIMULATORTABLEVIEW_H
#define SIMULATORTABLEVIEW_H
#include <QTableView>

/*!
 *  \~russian \brief Представление для таблицы "Моделирование" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс SimulatorTableView предназначен
 *  для отображения данных в виде таблицы и является наследником стандартного класса QTableView.
 */
 
class SimulatorTableView: public QTableView {
    Q_OBJECT
public:
    //! \~russian Конструктор класса. \details \~russian
    SimulatorTableView();
};

#endif // SIMULATORTABLEVIEW_H

/*@}*/
