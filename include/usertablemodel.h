/** @addtogroup Users
 * @{*/

/** @file */

#ifndef USETTABLEMODEL_H
#define USETTABLEMODEL_H

#include <QSqlTableModel>

/*!
 *  \~russian \brief Модель для работы с таблицей "Пользователи" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс UserTableModel представляет модель для таблицы
 *  "Закладки" (bookmark) и наследует интерфейс и реализацию стандартного класса QSqlTableModel библиотеки Qt.
 *  Он используется для соединения с истоником данных (таблица "Пользователи" Каталога) и  организации стандартного
 *  интерфеса взаимодействия представлениям (на просмотр, редактирование и удаление).
 */
 
class UserTableModel: public QSqlTableModel {
Q_OBJECT
public:
    //! \~russian Конструктор класса. \details \~russian
    UserTableModel();

    //some params
};




#endif // USETTABLEMODEL_H

/*@}*/
