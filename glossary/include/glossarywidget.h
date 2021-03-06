/** @addtogroup Glossary
 * @{*/

/** @file */

#ifndef GLOSSARYWIDGET_H
#define GLOSSARYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QWidgetAction>
#include <QToolBar>
#include <QDateTime>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>
#include "glossaryview.h"
#include "glossarytablemodel.h"
#include "historywidget.h"

/*!
 *  \~russian \brief Виджет для работы с таблицей "Глоссарий" сверхбольшого высокодоступного цифрового каталога данных (Каталога). Явялется составялющей частью копоненты 3d Атласа "Глоссарий"
 *
 *  \~russian Располагет в своей рабочей области модель-представление таблицы "Глоссарий", состоящей из двух столбцов "Понятие" и "Описание". Позволяет осущетсвлять фильрацию по полю "Понятие", 
 *   и сохранять соотвесвующий запрос фильра в таблицу  "История Поиска"(history).
 */
 
class GlossaryWidget: public QWidget {
Q_OBJECT
private:
    GlossaryTableModel* glossModel;

    QLabel* glossaryLabel;
    QLineEdit* glossFilterNotion;
    QVBoxLayout* glossVBoxLayout;
    GlossaryView* glossTableview;
    int owner_;


public:
    //! \~russian Конструктор класса \~russian
    GlossaryWidget(QWidget * pwgt = 0);
    //! \~russian Деструктор класса \~russian
    ~GlossaryWidget();
    //! \~russian Метод для установления закрытого поля id \~russian
    void setOwner(int id);
    HistoryWidget* historyWidget;

signals:
    //! \~russian Сигнал, испускаемый для обновления таблицы atlas.history отображаемой в виджете \~russian
    void updateHistoryTable();
    //! \~russian Сигнал, испускаемый для вставки строки в таблицу atlas.history отображаемой в виджете \~russian
    void addRowinHistoryTable(int id, QString& request);

public slots:
    //! \~russian Отвечает за фильтрацию таблицы по полю "Понятие"
    void setFilterNotion(const QString& filter);
    //! \~russian Добавляет запись в таблицу "История поиска"
    void addHistoryRow();


};


#endif // GLOSSARYWIDGET_H

/*@}*/
