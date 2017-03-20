/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef SIMULATORWIDGET_H
#define SIMULATORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlQuery>
#include <QDate>
#include <QProcess>
#include <QTextEdit>
#include <QSqlError>

#include "simulatortableview.h"
#include "simulatortablemodel.h"
#include "simulatorstarter.h"
#include "shell.h"
#include "progressbar.h"

/*!
 *  \~russian \brief Виджет для работы с таблицей "Моделирование" сверхбольшого высокодоступного цифрового каталога данных (Каталога). Явялется составялющей частью копоненты 3d Атласа "Глоссарий"
 *
 *  \~russian Располагет в своей рабочей области модель-представление таблицы "Моделирование". Позволяет запускать работу симулятора с заданнной через конфигурационной файл настройками. Позволяет
 *   добавлять/удалять записи из БД 
 */
 
class SimulatorWidget: public QWidget {
    Q_OBJECT
private:
    QVBoxLayout*    modellingVLayout;
    QHBoxLayout*    modellingHLayout;
    QHBoxLayout*    buttonsLayout;
    QComboBox*      modellingComboBox;
    QLabel*         modellingLabel;
    QPushButton*    loadConfigrButton;
    QPushButton*    editConfigButton;

    QFileDialog*            modellingFileDialog;
    SimulatorTableModel*    model;
    SimulatorTableView* view;
    QSqlRelationalTableModel* simulatorTable;

    QString         simulatorName;
    QPushButton*    startButton;
    QPushButton*    stopButton;
    QSqlQuery       query;
    QSqlQueryModel  queryModel;

    SimpleArtificialShell*  shell;
    SimulatorStarter*       simulator;

	ProgressBar* progressBar_ {Q_NULLPTR};
	QTimer* timerForUpdateTable_ {Q_NULLPTR};
	//! Время в миллисекундах, через которое обновляется таблица и строка прогресса.
	int updateInterval_ {5000};

    int owner;
    int id;
	QString processID_;

public:
    //! \~russian Конструктор класса. \details \~russian
    SimulatorWidget(QWidget* pwgt = 0);

	//! Функция устанавливает время в миллисекундах, через которое обновляется таблица и строка прогресса. \details Изменяет приватное поле updateInterval_.
	void setUpdateInterval(int updateInterval) { updateInterval_ = updateInterval; }
	//! Функция возвращает время в миллисекундах, через которое обновляется таблица и строка прогресса. \details Читает приватное поле updateInterval_.
	const int& getUpdateInterval() const { return updateInterval_; }

signals:
    //! \~russian Сигнал, испускаемый для открытия файла конфигурации \~russian
    void setFile(QString filename);

public slots:
    //! \~russian Метод отвечающий за получение id симулятора из таблицы atlas.simulator, фильтрация которого происходит по полю  reference\~russian
    int  getSimulatorID(QString simulatorName);
    //! \~russian Метод отвечающий за открытие файла конфигурации  \~russian
   //void openConfigFile();
    //! \~russian Метод отвечающий за запуск симулятора моделирования  \~russian
    void startExpirement();
   //! \~russian Метод отвечающий за вставку записи в таблицу atlas.simulator  \~russian
    void insertRow();
    //! \~russian Метод отвечающий за отображение таблицы atlas.modelling, связанной с таблицей atlas.simulator и фильтрующий записи по полю owner   \~russian
    void showModellingtableModel(int idOwner);
    //! \~russian Метод отвечающий за перерисовку виджета после его изменения  \~russian
    void updateWidget();
    //! \~russian Метод отвечающий отображения виджета NotepadWidget  \~russian
    void openNotepad();
    //! \~russian Метод отвечающий получения имени файла открытого в  NotepadWidget  \~russian
    //void setFilenamefromNotepad(QString fielname);
    //! \~russian Метод отвечающий за остановку симулятора моделирования  \~russian
    void stopExperiment();

    void enableButtons(const QString text);
};
#endif // SIMULATORWIDGET_H

/*@}*/
