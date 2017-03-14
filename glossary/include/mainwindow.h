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

#include "glossary.h"
#include "userview.h"

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
    Glossary* glossary;
    QStatusBar* status;

    QCheckBox*    checkBoxSaveParams;
    QHBoxLayout*  checkBoxLayout;
    QLabel*       saveParamsLabel;
    QDialog* loginDialog;

public:
    MainWindow(QWidget* pwgt = 0);

signals:
    //! \~russian Сигнал испускаемый при проверке пользователя по паре пользователь/пароль   \~russian
    bool checkAccount(const QString &login, const QString &password, Glossary* glossary);
    //! \~russian Сигнал испускаемый при проверке пользователя по хешу   \~russian
    bool checkAccount(const QString &hash, Glossary* glossary);

public slots:
    //! \~russian Метод отвечающий за проверку пользователя  \~russian
    void loginUser();
    //! \~russian Метод отвечающий за прориосвку главного окна  \~russian
    void showWidget();
    //! \~russian Метод отвечающий за прориосвку главного окна  \~russian
    void showWidget(const QString& hash, const QString& name);
    //! \~russian Метод отвечающий за закрытие окна авторизации  \~russian
    void closeLoginDialog();
    //! \~russian Метод отвечающий за визуализации ошибки при аторизации пользователя  \~russian
    void showLoginError();
};

#endif // MAINWINDOW_H
