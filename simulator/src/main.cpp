#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>
#include <QtGui>
#include <QTableView>
#include <QSqlDatabase>
#include <QtSql>
#include <QtSql/QSqlDatabase>

#include "simulatorwidget.h"
#include "mainwindow.h"
#include "startwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "Sql Driver" << QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	//db.setHostName("atlas3d.dev.informika.ru");
	db.setHostName("85.143.2.188");
    db.setDatabaseName("atlas3d");
    db.setUserName("atlas3d");
	//db.setPassword("fnkfc3l");
	//bool connectioncheck = db.open("atlas3d","fnkfc3l");
	bool connectioncheck = db.open("atlas3d","");

    if (connectioncheck == true){
        qDebug() << "Connection to database established." << endl;
    } else {
        qDebug() << "Error for database " << db.databaseName() << " :" << db.lastError().text() << endl;
    }

    StartWindow widget;
    widget.setApp(&app);
    widget.show();

    return app.exec();
}
