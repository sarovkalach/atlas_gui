/** @addtogroup Start
 * @{*/

/* @file */

#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTranslator>
#include <QApplication>
#include "mainwindow.h"
 /*!
 *  \~russian \brief Представляет собой всплывающее окно с выбором языка. Заупускает компоненты 3d Атласа с выбранным языком интерфейса. Наследник QDialog
 *
 *  
 */

class StartWindow: public QDialog {
    Q_OBJECT
private:
    QComboBox*    languageComboBox;
    QLabel*       languageLabel;
    QPushButton*  okButton;
    QPushButton*  cancelButton;

    QString       language;
    QVBoxLayout*  startLayout;
    QHBoxLayout*  buttonsLayout;
    QApplication* app;
    MainWindow*   main;
    QTranslator*  translator;

public:
    //! \~russian Конструктор класса. \details \~russian
    StartWindow(QWidget *pwgt=0);
   //! \~russian Вспомогателный метод, используемый для обеспечения мультилингвистичности приложения \~russian
    void setApp(QApplication* applictaion);

public slots:
   //! \~russian Метод, устанавливающий язык приложения \~russian
    void setLanguage(const QString);
    //! \~russian Метод отвечающий за старт приложения(стартует mainWindow)  \~russian
    void startUp();

};

#endif // STARTWINDOW_H

/*@}*/
