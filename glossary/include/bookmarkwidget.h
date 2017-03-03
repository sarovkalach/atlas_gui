/** @addtogroup Bookmark
 * @{*/

/** @file */

#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "bookmarktablemodel.h"
#include "bookmarkview.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>

/*!
 *  \~russian \brief Виджет для работы с таблицей "Закладки" сверхбольшого высокодоступного цифрового каталога данных (Каталога). Явялется составялющей частью копоненты 3d Атласа "Глоссарий"
 *
 *  \~russian Располагет в своей рабочей области модель-представление таблицы "Заметки", состоящей из двух столбцов "Название" и "Ссылки". Позволяет втавлять/удалять записи в БД нажатием конпок
 *   "Вставить/Удалить", предварительно заполнив поля "Понятие" и "Ссылка" 
 *   
 */
 
class BookmarkWidget: public QWidget {
Q_OBJECT

private:
    BookmarkTableModel* bookmarkTableModel;
    BookmarkView* bookmarkView;
    QVBoxLayout* bookmarkLayout;
    QHBoxLayout* buttonsLayout;

    QGridLayout* noteGrid;

    QLineEdit* bookmarkLineEditName;
    QLineEdit* bookmarkLineEditReference;

    QLabel* bookmarkLabelReference;
    QLabel* bookmarkLabellName;

    QPushButton* insertButton;
    QPushButton* deleteButton;

    int idOwner;
    QSqlQueryModel queryModel;
    QSqlQuery query;
    
public:
    //! \~russian Конструктор класса. \details \~russian
    BookmarkWidget(QWidget * pwgt = 0);
    //! \~russian Деструктор класса. \details \~russian
    ~BookmarkWidget();

public slots:
    //! \~russian Метод отвечающий за отображение таблицы atlas.bookmark и фильтрующий записи по полю owner   \~russian
    void showBookmarkTable(const int &owner);
   //! \~russian Метод отвечающий за удаление записи в таблицу atlas.bookmark  \~russian    
    void deleteRow();
   //! \~russian Метод отвечающий за вставку записи в таблицу atlas.bookmark  \~russian    
    void insertRow();
};

#endif // BOOKMARKWIDGET_H

/*@}*/
