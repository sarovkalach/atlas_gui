/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef SIMULATORTABLEMODEL_H
#define SIMULATORTABLEMODEL_H

#include <QSqlRelationalTableModel>

/*!
 *  \~russian \brief Модель для работы с таблицей "Моделирование" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс SimulatorTableModel представляет модель для таблицы
 *  "Моделирование" (modelling) и наследует интерфейс и реализацию стандартного класса QSqlRelationTableModel библиотеки Qt.
 *  Он используется для соединения с источником данных (таблица "Моделирование" Каталога) и  организации стандартного
 *  интерфеса взаимодействия представлениям (на просмотр, редактирование и удаление).
 */
 
class SimulatorTableModel: public QSqlRelationalTableModel {
public:
    //! \~russian Конструктор класса. \details \~russian
    SimulatorTableModel();
};

#endif // SIMULATORTABLEMODEL_H

/*@}*/
