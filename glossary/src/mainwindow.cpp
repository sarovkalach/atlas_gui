#include "mainwindow.h"

MainWindow::MainWindow(QWidget *pwgt)
    : QMainWindow(pwgt)
{

    menuAccount = new QMenu(tr("&Account"));
    menuBar()->addMenu(menuAccount);
    QAction *loginAction = new QAction("&Login", 0);
    loginAction->setText(tr("Login"));
    loginAction->setShortcut(QKeySequence("SHIFT+L"));
    menuAccount->addAction(loginAction);
    menuHelp = new QMenu(tr("Help"));
    menuBar()->addMenu(menuHelp);

    glossary = new Glossary;
    glossary->hide();

    connect(loginAction, SIGNAL(triggered()), SLOT(loginUser()));
    this->setCentralWidget(glossary);

    this->show();
}

void MainWindow::loginUser() {

    QDialog* loginDialog = new QDialog(this);
    QVBoxLayout* loginLayout = new QVBoxLayout;

    QLabel* userLabel = new QLabel();
    userLabel->setText(tr("User"));
    userLine = new QLineEdit();

    QLabel* passwordLabel = new QLabel();
    passwordLabel->setText(tr("Password"));
    passwordLine = new QLineEdit();

    QHBoxLayout* buttonsLayout = new QHBoxLayout;

    QPushButton* okButton = new QPushButton();
    okButton->setText(tr("Ok"));
    QPushButton* cancelButton = new QPushButton();
    cancelButton->setText(tr("Cancel"));

    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    loginLayout->addWidget(userLabel);
    loginLayout->addWidget(userLine);
    loginLayout->addWidget(passwordLabel);
    loginLayout->addWidget(passwordLine);
    loginLayout->addLayout(buttonsLayout);
    loginDialog->setLayout(loginLayout);

    checkBoxSaveParams = new QCheckBox;
    checkBoxLayout = new QHBoxLayout;
    saveParamsLabel = new QLabel;
    saveParamsLabel->setText(tr("Save session"));
    checkBoxLayout->addWidget(checkBoxSaveParams);
    checkBoxLayout->addWidget(saveParamsLabel);
    loginLayout->addLayout(checkBoxLayout);

    connect(okButton, SIGNAL(clicked()), SLOT(showWidget()));
    connect(okButton, SIGNAL(clicked()), loginDialog, SLOT(close()));
    connect(cancelButton, SIGNAL(clicked()), loginDialog,SLOT(close()));

    loginDialog->exec();

}




void MainWindow::showWidget() {

    userView = new UserView;
    QString name;
    QString pass;

    name = userLine->text();
    pass = passwordLine->text();
    userView->mark = this->checkBoxSaveParams->checkState();

    QObject::connect( this, SIGNAL(checkAccount(const QString&, const QString&, Glossary*)), userView, SLOT(checkID(const QString&, const QString&, Glossary*)) ); // USER table
    emit checkAccount(name, pass, glossary);
    QObject::disconnect( this, SIGNAL(checkAccount(const QString&, const QString&, Glossary*)), userView, SLOT(checkID(const QString&, const QString&, Glossary*)) ); // USER table

    statusBar()->showMessage(QString("Login: %1").arg(this->userLine->text()), 4000000);

    this->setGeometry(0, 0, 1024, 768);
    this->show();

}

void MainWindow::showWidget(const QString& hash, const QString& login) {

    userView = new UserView;

    QObject::connect( this, SIGNAL(checkAccount(const QString&, Glossary*)), userView, SLOT(checkID(const QString&, Glossary*)) ); // USER table
    emit checkAccount(hash, glossary);
    QObject::disconnect( this, SIGNAL(checkAccount(const QString&, Glossary*)), userView, SLOT(checkID(const QString&, Glossary*)) ); // USER table

    statusBar()->showMessage(QString("Login: %1").arg(login), 4000000);

    this->setGeometry(0, 0, 1024, 768);
    this->show();
    //this->setFixedSize(800, 600);

}
