#include "userview.h"

UserView::UserView() {
    this->hideColumn(0);
    mark = false;
    connect(this, SIGNAL(saveSessionSignal(const QString& ,const QString&, Glossary*)), this, SLOT(saveSessionSlot(const QString&,const QString&, Glossary*)));
}


UserView::~UserView() {
    ;
}


bool UserView::checkID(const QString &login, const QString &password, Glossary* glossary) {
    QString pass;

    connect(this, SIGNAL(closeDialogSignal()), this->parentWidget(), SLOT(closeLoginDialog()));
    connect(this, SIGNAL(showLoginErrorSignal()), this->parentWidget(), SLOT(showLoginError()));
    connect(this, SIGNAL(updateTopWidget()), glossary, SLOT(updateWidget()));

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
                glossary->parentWidget()->show();

                // Draw glossary widget
                emit updateTopWidget();
                emit saveSessionSignal(login, password, glossary);
                emit closeDialogSignal();
                return true;
            }
            else {
                qDebug() << "Name or pass is wrong(pass)";
                emit showLoginErrorSignal();
                return false;
            }
    }

    else {
        qDebug() << "Name or pass is wrong";
        emit showLoginErrorSignal();
        return false;
    }
    emit closeDialogSignal();
    disconnect(this, SIGNAL(closeDialogSignal()), this->parentWidget(), SLOT(closeLoginDialog()));
    disconnect(this, SIGNAL(showLoginErrorSignal()), this->parentWidget(), SLOT(showLoginError()));

    return false;
}


bool UserView::checkID(const QString &hash, Glossary* glossary) {
    QString pass;
    QString login;
    QString hashAccount;

    connect(this, SIGNAL(closeDialogSignal()), this->parentWidget(), SLOT(closeLoginDialog()));
    connect(this, SIGNAL(showLoginErrorSignal()), this->parentWidget(), SLOT(showLoginError()));
    connect(this, SIGNAL(updateTopWidget()), glossary, SLOT(updateWidget()));

    QFile file("myfile.txt");

    if( file.open(QIODevice::ReadOnly) ) {

        login = file.readLine();
        login = login.remove(login.size()-1, 1);

        pass = file.readLine();
        pass = pass.remove(pass.size()-1, 1);

        hashAccount = file.readLine();
        hashAccount = hashAccount.remove(hashAccount.size()-1, 1);

        query.prepare("SELECT * FROM atlas.users WHERE login = :login ");//
        query.bindValue(":login", login);
        query.exec();

        if( query.first() ) {
            const int owner = query.value(0).toInt();
            glossary->noteWidget->showNoteTable(owner);
            glossary->bookmarkWidget->showBookmarkTable(owner);
            glossary->historyWidget->showHistoryTable(owner);
            glossary->loginName = login;;
            emit closeDialogSignal();
        }

        QString hashAccount = QString(QCryptographicHash::hash(((login + pass + "⁠⁠⁠HerVamVsem")).toStdString().c_str(),QCryptographicHash::Md5).toHex());

        if ( hashAccount == hash) {

            glossary->parentWidget()->show();
            emit updateTopWidget();
            file.close();
            return true;
        }

    }

    else {
        qDebug() << "Access denied,file not open";
        emit showLoginErrorSignal();
    }

    disconnect(this, SIGNAL(closeDialogSignal()), this->parentWidget(), SLOT(closeLoginDialog()));
    disconnect(this, SIGNAL(showLoginErrorSignal()), this->parentWidget(), SLOT(showLoginError()));

    return false;

}


void UserView::saveSessionSlot(const QString& login,  const QString& pass, Glossary* glossary) {

    connect(this, SIGNAL(closeDialogSignal()), this->parentWidget(), SLOT(closeLoginDialog()));
    connect(this, SIGNAL(showLoginErrorSignal()), this->parentWidget(), SLOT(showLoginError()));
    connect(this, SIGNAL(updateTopWidget()), glossary, SLOT(updateWidget()));

    QFile file("myfile.txt");
    // get hash from (login + pass)
    QString hash = QString(QCryptographicHash::hash(((login + pass + "⁠⁠⁠HerVamVsem")).toStdString().c_str(),QCryptographicHash::Md5).toHex());


    if ( !file.exists() && file.size() > 0 ) {

        file.open(QIODevice::ReadWrite);
        if ( mark ) {
            writeAccount(file, login, pass, hash);
            glossary->parentWidget()->show();
        }
        emit updateTopWidget();
    } else {
        file.open(QIODevice::ReadWrite);
        while ( !file.atEnd()) {
            QString tmp = file.readLine();
            tmp = tmp.remove(tmp.size()-1, 1);
            if ( tmp == hash )
                emit updateTopWidget();
           }
        if ( mark ) {

            emit closeDialogSignal();
            glossary->parentWidget()->show();
            writeAccount(file, login, pass, hash);
        }
    }
    emit closeDialogSignal();
    disconnect(this, SIGNAL(closeDialogSignal()), this->parentWidget(), SLOT(closeLoginDialog()));
    disconnect(this, SIGNAL(showLoginErrorSignal()), this->parentWidget(), SLOT(showLoginError()));
}

void UserView::writeAccount(QFile &file, QString login, QString pass, QString hash) {
    file.seek(0);
    QTextStream stream(&file);
    stream << login << endl;
    stream << pass << endl;
    stream << hash << endl;
    file.close();
}
