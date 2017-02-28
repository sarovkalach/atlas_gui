/** @addtogroup Integrate
 * @{*/

/** @file */

#ifndef INTEGRATEWIDGET_H
#define INTEGRATEWIDGET_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlQuery>
#include "notepadwidget.h"
#include <QDate>
#include <QProcess>
#include <QTextEdit>
#include <QSqlError>
#include "integratortablemodel.h"
#include "integratetableview.h"

/*!
 *  \~russian \brief Виджет для работы с таблицей "Интегрирование" сверхбольшого высокодоступного цифрового каталога данных (Каталога)
 *
 *  \~russian Располагет в своей рабочей области модель-представление таблицы "Интегрирование". Обеспечивает дуплексную связь между экспериментами и Симулятором.Позволяет
 *   добавлять/удалять записи из БД.
 */
 
class IntegrateWidget: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* modellingVLayout;
    QHBoxLayout* modellingHLayout;
    QHBoxLayout* buttonsLayout;
    QComboBox*   modellingComboBox;
    QLabel*      modellingLabel;
    QLineEdit*   modellingLineEdit;
    QPushButton* loadConfigrButton;
    QPushButton* editConfigButton;
    QFileDialog*     modellingFileDialog;
    IntegrateTableModel* model;
    IntegrateTableView* view;
    QSqlRelationalTableModel* simulatorTable;
    QString simulatorName;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* deleteButton;
    QSqlQuery query;
    QSqlQueryModel queryModel;
    NotepadWidget* notepad;

    int owner;
    int id;

public:
    //! \~russian Конструктор класса. \details \~russian
    IntegrateWidget(QWidget* pwgt = 0);

signals:
    //! \~russian Сигнал, испускаемый для открытия файла конфигурации \~russian
    void setFile(QString filename);

public slots:
    //! \~russian Вспомогательный метод использовался для отладки \~russian
    int  getSimulatorID(QString simulatorName);
    //! \~russian Метод отвечающий за открытие файла конфигурации \~russian    
    void openConfigFile();
    //! \~russian Метод отвечающий за старт эксперимента \~russian        
    void startExpirement();
    //! \~russian Метод отвечающий за вставку записи в таблицу atlas.integrate \~russian        
    void insertRow();
    //! \~russian Метод отвечающий за удаление записи в таблицу atlas.integrate \~russian            
    void removeRow();
    //! \~russian Метод отвечающий за отображение таблицы atlas.integrate   \~russian
    void showIntegrateTableModel(int idOwner);
    //! \~russian Метод отвечающий отображения виджета NotepadWidget  \~russian
    void updateWidget();
    //! \~russian Метод отвечающий отображения виджета NotepadWidget  \~russian    
    void openNotepad();
    //! \~russian Метод отвечающий получения имени файла открытого в  NotepadWidget  \~russian    
    void setFilenamefromNotepad(QString fielname);
};

#endif // INTEGRATEWIDGET_H

/*@}*/
