/** @addtogroup Bookmark
 * @{*/

/** @file */

#ifndef BOOKMARKVIEW_H
#define BOOKMARKVIEW_H

#include <QTableView>

/*!
 *  \~russian \brief Представление для таблицы "Закладки" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс BookmarkView предназначен
 *  для отображения данных в виде таблицы и является наследником стандартного класса QTableView.
 */
 
class BookmarkView: public QTableView {
public:
    //! \~russian Конструктор класса. \details \~russian
    BookmarkView();
    //! \~russian Деструктор класса. \details \~russian
    ~BookmarkView();
};

#endif // BOOKMARKVIEW_H

/*@}*/
