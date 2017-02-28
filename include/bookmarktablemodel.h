/** @addtogroup Bookmark
 * @{*/

/** @file */

#ifndef BOOKMARKTABLEMODEL_H
#define BOOKMARKTABLEMODEL_H

#include <QSqlTableModel>

/*!
 *  \~russian \brief Модель для работы с таблицей "Закладки" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс BookmarkTableModel представляет модель для таблицы
 *  "Закладки" (bookmark) и наследует интерфейс и реализацию стандартного класса QSqlTableModel библиотеки Qt.
 *  Он используется для соединения с истоником данных (таблица "Закладки" Каталога) и  организации стандартного
 *  интерфеса взаимодействия представлениям (на просмотр, редактирование и удаление).
 */
 
class BookmarkTableModel: public QSqlTableModel {
public:
    //! \~russian Конструктор класса. \details \~russian
    BookmarkTableModel();
};

#endif // BOOKMARKTABLEMODEL_H

/*@}*/
