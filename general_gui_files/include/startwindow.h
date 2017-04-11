/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QTranslator>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

//! Функция соединяется с базой данных по параметрам по умолчанию.
void connectToTheDataBase(QApplication& app);

 /*!
 *  \~russian \brief Окно с выбором языка.
 *
 * Представляет собой диалоговое окно с выбором языка.
 * Устанавливает язык для приложения в соответствии с
 * выбором пользователя.
 *
 * Схема использования:
 * \code
 *
 *  QApplication app(argc, argv);
 *
 *	int res = StartWindow::createStartWindow(&app);
 *	if (res == QDialog::Rejected){
 *		app.closeAllWindows();
 *		return app.exec();
 *	}
 *
 *  //выполнение самой программы
 *  //...
 *
 *  return app.exec();
 *
 * \endcode
 *  
 */
class StartWindow: public QDialog {
    Q_OBJECT
private:
	//! Выпадающее окно с выбором языка (English или Russian)
    QComboBox*    languageComboBox;
	//! Надпись "Choose Language:"
    QLabel*       languageLabel;
	//! Кнопка "Ok". Запускает выполнение программы.
    QPushButton*  okButton;
	//! Кнопка "Cancel". Останавливает выполнение программы.
    QPushButton*  cancelButton;

	//! Главный менеджер размещения.
    QVBoxLayout*  startLayout;
	//! Менеджер размещения для кнопок.
    QHBoxLayout*  buttonsLayout;
	//! Входной параметр - приложение, у которого мы меняем язык в зависимости от выбора пользователя.
	QApplication* app;
	//! Переводчик, который устанавливает язык.
    QTranslator*  translator;

    //! \~russian Конструктор класса. \details \~russian
	explicit StartWindow(QWidget *pwgt=0);


   //! \~russian Вспомогателный метод, используемый для обеспечения мультилингвистичности приложения \~russian
	void setApp(QApplication* applictaion);

public:
	//! Статический метод, который меняет язык. позволяет использовать окно как диалоговое.
	static int createStartWindow(QApplication* application){
		StartWindow sw;
		sw.setApp(application);
		return sw.exec();
	}

private slots:
   //! \~russian Метод, устанавливающий язык приложения \~russian
    void setLanguage(const QString);
    //! \~russian Метод отвечающий за старт приложения(стартует mainWindow)  \~russian
    void startUp();

};

#endif // STARTWINDOW_H

/*@}*/
