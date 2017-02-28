/** @addtogroup Note
 * @{*/

/** @file */

#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QDateTime>
#include <QSqlQuery>
#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QHeaderView>

#include "noteview.h"
#include "notetablemodel.h"

/*!
 *  \~russian \brief Виджет для работы с таблицей "Заметки" сверхбольшого высокодоступного цифрового каталога данных (Каталога). Явялется составялющей частью копоненты 3d Атласа "Глоссарий"
 *
 *  \~russian Располагет в своей рабочей области модель-представление таблицы "Заметки", состоящей из двух столбцов "Дата" и "Содержимое". Позволяет втавлять/удалять записи в БД нажатием конпок
 *   "Вставить/Удалить", предварительно заполнив поле "Содержимое"
 *   и сохранять соотвесвующий запрос фильра в таблицу  "История Поиска"(history).
 */
 
class NoteWidget: public QWidget {
Q_OBJECT
private:
    NoteTableModel* noteModel;
    NoteView* noteView;
    QLabel* noteNameLabel;
    QLabel* noteContentLabel;
    QGridLayout* gridLayout;
    QLineEdit* noteNameLineEdit;
    QLineEdit* noteContentLineEdit;
    QPushButton* insertButton;
    QPushButton* deleteButton;
    QSqlQuery query;
    QSqlQueryModel queryModel;

public:
    //! \~russian Конструктор класса. \details \~russian
    NoteWidget(QWidget * pwgt = 0);
    int idOwner;

public slots:
    //! \~russian Метод отвечающий за отображение таблицы atlas.note с фильтрацией по полю owner \~russian        
    void showNoteTable(const int &owner);
    //! \~russian Метод отвечающий за вставку записи в таблицу atlas.note \~russian            
    void insertRow();
    //! \~russian Метод отвечающий за удаление записи в таблицу atlas.note \~russian                
    void deleteRow();
};


#endif // NOTEWIDGET_H

/*@}*/
