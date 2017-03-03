/** @addtogroup Note
 * @{*/

/** @file */

#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QTableView>

/*!
 *  \~russian \brief Представление для таблицы "Заметки" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс NoteView предназначен
 *  для отображения данных в виде таблицы и является наследником стандартного класса QTableView.
 */
 
class NoteView: public QTableView {
public:
    //! \~russian Конструктор класса. \details \~russian
    NoteView();
    //! \~russian Деструктор класса. \details \~russian
    ~NoteView();
};

#endif // NOTEVIEW_H

/*@}*/
