/** @addtogroup Glossary
 * @{*/

/** @file */

#ifndef GLOSSARYVIEW_H
#define GLOSSARYVIEW_H
#include <QTableView>

/*!
 *  \~russian \brief Представление для таблицы "Глоссарий" сверхбольшого высокодоступного цифрового каталога данных (Каталога).
 *
 *  \~russian Программирование графического интерфейса пользователя для Каталога
 *  выполнена в рамках архитектуры "Модель-представление". Класс GlossaryView предназначен
 *  для отображения данных в виде таблицы и является наследником стандартного класса QTableView.
 */
class GlossaryView: public QTableView {
Q_OBJECT
public:
    //! \~russian Конструктор класса. \details \~russian ОПИСАТЬ ЧТО ДЕЛАЕТ КОНСТРУКТОР, ЕСЛИ ЧТО-ТО ДЕЛАЕТ
    GlossaryView();
    //! \~russian Деструктор класса. \details \~russian .........
    ~GlossaryView();
};


#endif // GLOSSARYVIEW_H

/*@}*/
