/** @addtogroup Integrate
 * @{*/

/** @file */

#ifndef INTEGRATORTABLEMODEL_H
#define INTEGRATORTABLEMODEL_H

#include <QSqlRelationalTableModel>

/*!
 *  \~russian \brief Модель для работы с таблицей "Интеграция" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс IntegratorTableModel представляет модель для таблицы
 *  "Интеграция" (integrate) и наследует интерфейс и реализацию стандартного класса QSqlRelationTableModel библиотеки Qt.
 *  Он используется для соединения с источником данных (таблица "Интеграция" Каталога) и  организации стандартного
 *  интерфеса взаимодействия представлениям (на просмотр, редактирование и удаление).
 */
 
class IntegrateTableModel: public QSqlRelationalTableModel {
public:
    //! \~russian Конструктор класса. \details \~russian
    IntegrateTableModel();
};

#endif // INTEGRATORTABLEMODEL_H

/*@}*/
