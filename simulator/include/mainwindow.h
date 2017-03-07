#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QStatusBar>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QTableWidget>
#include <QMenu>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTranslator>
#include <QStatusBar>
#include <QCheckBox>

#include "userview.h"
#include "simulatorwidget.h"

class MainWindow: public QMainWindow {
Q_OBJECT

private:
    UserView* userView;
    QTranslator* translator;
    QLineEdit* passwordLine;
    QLineEdit* userLine;
    QMenu* menuAccount;
    QMenu* menuLanguage;
    QMenu* menuHelp;
    QStatusBar* status;
    
    QCheckBox*    checkBoxSaveParams;
    QHBoxLayout*  checkBoxLayout;
    QLabel*       saveParamsLabel;
    QDialog* loginDialog;

public:
    MainWindow(QWidget* pwgt = 0);
    SimulatorWidget* simulatorWidget;

signals:
    //! \~russian Сигнал отвечающий проверку аккаунта по хэшу \~russian
    bool checkAccount(const QString &hash, SimulatorWidget* simulatorWidget);
    
    //! \~russian Сигнал отвечающий проверку аккаунта по паре пароль/пользователь \~russian
    bool checkAccount(const QString &login, const QString &pass, SimulatorWidget* simulatorWidget);



public slots:
    void loginUser();
    void showWidget();
    void showWidget(const QString& hash, const QString& name);
    void closeLoginDialog();
    void showLoginError();
};

#endif // MAINWINDOW_H
