#include "userview.h"

UserView::UserView() {

    this->hideColumn(0);
    mark = false;

}


UserView::~UserView() {
    ;
}


bool UserView::checkID(const QString &login, const QString &password, Glossary* glossary) {

    QString pass;

    query.prepare("SELECT * FROM atlas.users WHERE login = :login ");//
    query.bindValue(":login", login);
    query.exec();

    if( query.first() ) {
        pass = query.value(2).toString();
            if ( pass == password ) {
                qDebug() << "Account is TRUE";

                const int owner = query.value(0).toInt();

                glossary->noteWidget->showNoteTable(owner);
                glossary->bookmarkWidget->showBookmarkTable(owner);
                glossary->historyWidget->showHistoryTable(owner);
                glossary->loginName = login;

                // Draw glossary widget
                connect(this, SIGNAL(updateTopWidget()), glossary, SLOT(updateWidget()));
                emit updateTopWidget();
                emit saveSessionSignal(login, password, glossary);

                return true;
            }
            else {
                qDebug() << "Account is FALSE";
                return false;
            }
    }

    else {
        qDebug() << "Access denied";
        return false;
    }
}


bool UserView::checkID(const QString &hash, Glossary* glossary) {
    QString pass;
    QString login;
    QString hashAccount;

    connect(this, SIGNAL(updateTopWidget()), glossary, SLOT(updateWidget()));
    QFile file("/home/kalach/Qt/GUI/myfile.txt");

    if( file.open(QIODevice::ReadOnly) ) {

        login = file.readLine();
        login = login.remove(login.size()-1, 1);
        qDebug() << login;

        pass = file.readLine();
        pass = pass.remove(pass.size()-1, 1);
        qDebug() << pass;

        hashAccount = file.readLine();
        hashAccount = hashAccount.remove(hashAccount.size()-1, 1);
        qDebug() << hashAccount << " | " << hash << endl;

        query.prepare("SELECT * FROM atlas.users WHERE login = :login ");//
        query.bindValue(":login", login);
        query.exec();

        if( query.first() ) {
            const int owner = query.value(0).toInt();
            glossary->noteWidget->showNoteTable(owner);
            glossary->bookmarkWidget->showBookmarkTable(owner);
            glossary->historyWidget->showHistoryTable(owner);
            glossary->loginName = login;;
        }

        QString hashAccount = QString(QCryptographicHash::hash(((login + pass + "⁠⁠⁠HerVamVsem")).toStdString().c_str(),QCryptographicHash::Md5).toHex());

        if ( hashAccount == hash) {

            emit updateTopWidget();
            file.close();
            return true;
        }

    }

    else {
        qDebug() << "Access denied,file not open";
    }
    return false;

}


void UserView::saveSessionSlot(const QString& login,  const QString& pass, Glossary* glossary) {

    connect(this, SIGNAL(updateTopWidget()), glossary, SLOT(updateWidget()));
    QFile file("/home/kalach/Qt/GUI/myfile.txt");

    // get hash from (login + pass)
    QString hash = QString(QCryptographicHash::hash(((login + pass + "⁠⁠⁠HerVamVsem")).toStdString().c_str(),QCryptographicHash::Md5).toHex());


    if ( !file.exists() ) {

        file.open(QIODevice::ReadWrite);
        if ( mark )
            writeAccount(file, login, pass, hash);
        emit updateTopWidget();
    } else {
        file.open(QIODevice::ReadWrite);
        while ( !file.atEnd()) {
            QString tmp = file.readLine();
            tmp = tmp.remove(tmp.size()-1, 1);
            if ( tmp == hash )
                emit updateTopWidget();
           }
        if ( mark )
            writeAccount(file, login, pass, hash);
    }

}

void UserView::writeAccount(QFile &file, QString login, QString pass, QString hash) {
    file.seek(0);
    QTextStream stream(&file);
    stream << login << endl;
    stream << pass << endl;
    stream << hash << endl;
    file.close();
}
