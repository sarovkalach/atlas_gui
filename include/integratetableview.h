/** @addtogroup Integrate
 * @{*/

/** @file */

#ifndef INTEGRATETABLEVIEW_H
#define INTEGRATETABLEVIEW_H

#include <QTableView>

/*!
 *  \~russian \brief Представление для таблицы "Интегрирование" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс IntegrateTableView предназначен
 *  для отображения данных в виде таблицы и является наследником стандартного класса QTableView.
 */
 
class IntegrateTableView: public QTableView {
public:
    //! \~russian Конструктор класса. \details \~russian
    IntegrateTableView();
};

#endif // INTEGRATETABLEVIEW_H

/*@}*/
