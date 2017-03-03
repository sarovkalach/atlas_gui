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
#include "integratewidget.h"

class MainWindow: public QMainWindow {
Q_OBJECT

private:
    IntegrateWidget* integrateWidget;
    UserView* userView;
    QTranslator* translator;
    QLineEdit* passwordLine;
    QLineEdit* userLine;
    QMenu* menuAccount;
    QMenu* menuLanguage;
    QMenu* menuHelp;

    QCheckBox*    checkBoxSaveParams;
    QHBoxLayout*  checkBoxLayout;
    QLabel*       saveParamsLabel;


public:
    MainWindow(QWidget* pwgt = 0);

signals:
    //! \~russian Сигнал отвечающий проверку аккаунта по хэшу \~russian
    bool checkAccount(const QString &hash, IntegrateWidget* integrateWidget);

    //! \~russian Сигнал отвечающий проверку аккаунта по паре пароль/пользователь \~russian
    bool checkAccount(const QString &login, const QString &password, IntegrateWidget* integrateWidget);




public slots:
    void loginUser();
    void showWidget();
    void showWidget(const QString& hash, const QString& name);
};

#endif // MAINWINDOW_H