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
#include "simulatorwidget.h"
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
    Glossary* glossary;

    QCheckBox*    checkBoxSaveParams;
    QHBoxLayout*  checkBoxLayout;
    QLabel*       saveParamsLabel;


public:
    MainWindow(QWidget* pwgt = 0);
    SimulatorWidget* simulatorWidget;

signals:
    bool checkAccount(const QString &login, const QString &password, Glossary* glossary);
    bool checkAccount(const QString &hash, SimulatorWidget* simulatorWidget);
    bool checkAccount(const QString name, const QString login, SimulatorWidget* simulatorWidget);

    bool checkAccount(const QString &login, const QString &password, IntegrateWidget* integrateWidget);


public slots:
    void loginUser();
    void showWidget();
    void showWidget(const QString& hash, const QString& name);
};

#endif // MAINWINDOW_H
