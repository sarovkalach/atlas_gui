#include "userview.h"

UserView::UserView() {

    this->hideColumn(0);
    mark = false;

}


UserView::~UserView() {
    ;
}


bool UserView::checkID(const QString &hash, IntegrateWidget* integrateWidget) {
    QString pass;
    QString login;
    QString hashAccount;

    connect(this, SIGNAL(updateTopWidget()), integrateWidget, SLOT(updateWidget()));
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
            QString pass = query.value(2).toString();
            integrateWidget->showIntegrateTableModel(owner);
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

bool UserView::checkID(const QString &login, const QString &password, IntegrateWidget *integrateWidget) {
    QString pass;
    connect(this, SIGNAL(saveSessionSignal(const QString& ,const QString&, IntegrateWidget*)), this, SLOT(saveSessionSlot(const QString&,const QString&, IntegrateWidget*)));

    query.prepare("SELECT * FROM atlas.users WHERE login = :login ");//
    query.bindValue(":login", login);
    query.exec();

    if( query.first() ) {
        pass = query.value(2).toString();
            if ( pass == password ) {
                qDebug() << "Account is TRUE";

                const int owner = query.value(0).toInt();
                integrateWidget->showIntegrateTableModel(owner);

                //Save session
                // login after check saved session
                emit saveSessionSignal(login, password, integrateWidget);
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


void UserView::editID(const QString& login) {

    userTableModel.setFilter( QString("login='%1'").arg(login) );
    this->setModel(&userTableModel);

    this->hideColumn(0);
    this->hideColumn(2);
    this->show();
}


void UserView::saveSessionSlot(const QString& login,  const QString& pass, IntegrateWidget* integrateWidget) {

    connect(this, SIGNAL(updateTopWidget()), integrateWidget, SLOT(updateWidget()));
    QFile file("myfile.txt");

    // get hash from (login + pass)
    QString hash = QString(QCryptographicHash::hash(((login + pass + "⁠⁠⁠HerVamVsem")).toStdString().c_str(),QCryptographicHash::Md5).toHex());

    if ( file.exists() && file.size() > 0) {
        file.open(QIODevice::ReadWrite);
        while ( !file.atEnd()) {
           QString tmp = file.readLine();
            tmp = tmp.remove(tmp.size()-1, 1);
            if ( tmp == hash )
                emit updateTopWidget();
        }
        if ( mark )
            writeAccount(file, login, pass, hash);
    } else {
        file.open(QIODevice::ReadWrite);
        if ( mark )
            writeAccount(file, login, pass, hash);
        emit updateTopWidget();
    }
    qDebug() << this << endl;
}

void UserView::writeAccount(QFile &file, QString login, QString pass, QString hash) {
    file.seek(0);
    QTextStream stream(&file);
    stream << login << endl;
    stream << pass << endl;
    stream << hash << endl;
    file.close();
}
