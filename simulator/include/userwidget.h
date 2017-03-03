/** @addtogroup Users
 * @{*/

/** @file */

#ifndef USERWIDGET
#define USERWIDGET

#include <QVBoxLayout>
#include <QSqlQuery>
#include "userview.h"

/*!
 *  \~russian \brief Виджет для работы с таблицей "Пользователи" сверхбольшого высокодоступного цифрового каталога данных (Каталога).Использовался для отладки логирования пользователей
 *
 
 *   
 */

class UserWidget: public QWidget {
Q_OBJECT

private:
    UserView* userView;
    UserTableModel* userModel;
    QVBoxLayout* userVBoxLayout;

public:
    //! \~russian Конструктор класса. \details \~russian
    UserWidget(QWidget * pwgt = 0);
   //! \~russian Деструктор класса. \details \~russian
   ~UserWidget();

};

#endif // USERWIDGET


/*@}*/
