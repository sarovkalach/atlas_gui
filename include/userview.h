/** @addtogroup Users
 * @{*/

/** @file */

#ifndef USERVIEW_H
#define USERVIEW_H

#include <QTableView>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDebug>
#include <QFile>
#include <QCryptographicHash>
#include "usertablemodel.h"
#include "simulatorwidget.h"

/*!
 *  \~russian \brief Класс отвечающий за логирование пользователя по паре(имя/пользователь)
 *
  */
class UserView: public QTableView {
Q_OBJECT
private:
    QSqlQuery query;
    QSqlQueryModel queryModel;
    UserTableModel userTableModel;

public:
    //! \~russian Конструктор класса. \details \~russian
    UserView();
    //! \~russian Деструктор класса. \details \~russian
    ~UserView();
    //! \~russian Метод для записи данных аккаунта в файл. \details \~russian
    void writeAccount(QFile &file, QString login, QString pass, QString hash);
    //! \~russian поля для отметки QCheckBox, проверяющий истинность бытия \details \~russian
    bool         mark;

public slots:

    //! \~russian Метод, проверяющий пользователя по паре (имя/пользователь) для  виджета SimulatorWidget \~russian
    bool checkID(const QString &login, const QString &password, SimulatorWidget* simulatorWidget);

    //! \~russian Метод, проверяющий пользователя по хэшу \~russian
    bool checkID(const QString &hash, SimulatorWidget* simulatorWidget);

    //! \~russian Вспомогательный метод, использовался для отладки \~russian
    void editID(const QString &login);

    //! \~russian Метод, отвечающий за сохранение сессии в кюшированном виде \~russian
    void saveSessionSlot(const QString& login, const QString& pass, SimulatorWidget* simulatorWidget);

signals:
    //! \~russian Сигнал отвечающий за перерисовку главного виджета, после его изменения \~russian
    void updateTopWidget();
    //! \~russian Сигнал, сохраняющий хэш  \~russian
    void saveSessionSignal(const QString& login, const QString& pass, SimulatorWidget* simulatorWidget);

};

#endif // USERVIEW_H

/*@}*/
