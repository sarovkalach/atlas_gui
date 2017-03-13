/** @addtogroup History
 * @{*/

/** @file */

#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPushButton>
#include "historytablemodel.h"
#include "historyview.h"

/*!
 *  \~russian \brief Виджет для работы с таблицей "Глоссарий" сверхбольшого высокодоступного цифрового каталога данных (Каталога). Явялется составялющей частью копоненты 3d Атласа "Глоссарий"
 *
 *  \~russian Располагет в своей рабочей области модель-представление таблицы "История поиска", состоящей из двух столбцов "Дата" и "Запрос". Позволяет удалять записи из БД путем нажатия
 *  соответсвующей кнопки.
 */
 
class HistoryWidget: public QWidget {
Q_OBJECT
private:
    HistoryView* historyView;
    QVBoxLayout* historyLayout;
    QHBoxLayout* buttonsLayout;
    QPushButton* insertButton;
    QPushButton* deleteButton;
    QSqlQueryModel queryModel;
    QSqlQuery query;

    int idOwner_;

public:
    //! \~russian Конструктор класса. \details \~russian
    HistoryWidget(QWidget * pwgt = 0);
    HistoryTableModel* historyModel;

    ~HistoryWidget();

public slots:
    //! \~russian Метод отвечающий за вставку записи в таблицу atlas.history  \~russian
    void addRow(int owner, QString& request);
    //! \~russian Метод отвечающий за удаление записи из таблицы atlas.history  \~russian
    void deleteRow();
    //! \~russian Метод отвечающий за отображение таблицы atlas.history и фильтрующий записи по полю owner   \~russian
    void showHistoryTable(const int &owner);
    //! \~russian Метод отвечающий за перерисовку виджета после его изменения  \~russian    
    void updateHistoryTable();
};

#endif // HISTORYWIDGET_H

/*@}*/
