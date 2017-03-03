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
#include "notepadwidget.h"
#include "integratewidget.h"

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

    //QTranslator translator;

    //translator.load("/home/kalach/Qt/GUI/main_ru.qm");
    //app.installTranslator(&translator);

    //TopLevelWidget widget;
    //widget.historyWidget->showHistoryTable(1);
    //BookmarkWidget widget;
    //widget.showBookmarkTable(1);
    //NoteWidget widget;
    //widget.showNoteTable(1);
    //MainWindow widget;
    //widget.setApp(&app);
    //qDebug() <<  widget.translator->isEmpty();
    //HistoryWidget widget;
    //widget.showHistoryTable(1);
    //UserWidget widget;

    //GlossaryWidget widget;
    //widget.showNoteTable(2);
     //widget.show();
    //QPushButton hello(QPushButton::tr("Hello world!"));
    //hello.resize(100, 30);
    //hello.show();

    StartWindow widget;
    widget.setApp(&app);
    //widget.show();

    //IntegrateWidget widget;
    //SimulatorWidget widget;

    //NotepadWidget widget;
    widget.show();

    return app.exec();
}
