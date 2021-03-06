/** @addtogroup Glossary
 * @{*/

/** @file */

#ifndef GLOSSARY_H
#define GLOSSARY_H

#include <QWidget>
#include <QCloseEvent>
#include "historywidget.h"
#include "notewidget.h"
#include "bookmarkwidget.h"
#include "glossarywidget.h"

/*!
 *  \~russian \brief Виджет, представляющий собой главный виджет компоненты 3d Атласа "Глоссарий".
 *
 *  \~russian Располагет в своей рабочей области HistoryWidget, GlossaryWidget, NoteWidget, BookmarkWidget.
 */

class Glossary: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout*    verticalLayout;
    QHBoxLayout*    horizontalLayout;
    QGridLayout*    totalGrid;

public:
    //! \~russian Деструктор класса. \details \~russian
    Glossary(QWidget * pwgt = 0);
    ~Glossary();

    HistoryWidget*  historyWidget;
    NoteWidget*     noteWidget;
    BookmarkWidget* bookmarkWidget;
    GlossaryWidget* glossaryWidget;

    int idOwner;

public slots:
    //! \~russian Метод отвечающий за перерисовку виджета после его изменения  \~russian
    void updateWidget();
    //! \~russian Метод отвечающий за обновление отображаемой таблицы в виджете HistoryWidget  \~russian    
    void updateTableWidget();
};

#endif // GLOSSARY_H

/*@}*/
