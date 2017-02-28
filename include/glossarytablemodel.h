/** @addtogroup Glossary
 * @{*/

/** @file */

#ifndef GLOSSARYTABLEMODEL_H
#define GLOSSARYTABLEMODEL_H
#include <QSqlTableModel>

/*!
 *  \~russian \brief Модель для работы с таблицей "Глоссарий" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс GlossaryTableModel представляет модель для таблицы
 *  "Глоссарий" (glossary) и наследует интерфейс и реализацию стандартного класса QSqlTableModel библиотеки Qt.
 *  Он используется для соединения с истоником данных (таблица "Глоссарий" Каталога) и  организации стандартного
 *  интерфеса взаимодействия представлениям (на просмотр, редактирование и удаление).
 */
class GlossaryTableModel: public QSqlTableModel {
Q_OBJECT
public:
    //! \~russian Конструктор класса. \details \~russian ОПИСАТЬ ЧТО ДЕЛАЕТ КОНСТРУКТОР, ЕСЛИ ЧТО-ТО ДЕЛАЕТ
    GlossaryTableModel();
};

#endif // GLOSSARYTABLEMODEL_H

/*@}*/

