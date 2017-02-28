/** @addtogroup History
 * @{*/

/** @file */

#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QTableView>

/*!
 *  \~russian \brief Представление для таблицы "История поиска" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс HistoryView предназначен
 *  для отображения данных в виде таблицы и является наследником стандартного класса QTableView.
 */
 
class HistoryView: public QTableView {
public:
    //! \~russian Конструктор класса. \details \~russian
    HistoryView();
    //! \~russian Десструктор класса. \details \~russian    
    ~HistoryView();
};

#endif // HISTORYVIEW_H

/*@}*/
