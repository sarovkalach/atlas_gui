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
#include <QDebug>
#include <QSplitter>
#include <QMessageBox>

#include "simulatortableview.h"
#include "simulatortablemodel.h"
#include "simulatorstarter.h"
#include "../general_gui_files/include/shell.h"
#include "progressbar.h"

/*! \brief Диалоговое окно текстового редактора.
 *
 * Вызывает диалоговое окно с текстовым редактором (QTextEdit),
 * в котором изначально написан текст, который поступил в качестве
 * аргумента. Его же диалоговое окно и меняет при нажатии на кнопку "OK".
 *
 * Используется внутри SimulatorWidget для добавления описания.
 *
 * Схема использования:
 * \code
 *
 * QString test = "Hello!";
 * TextDialog::createTextDialog(QString& text);
 * qDebug()<<"user entered:";
 * qDebug()<<text;
 *
 * \endcode
 *
 */
class TextDialog : public QDialog{
	Q_OBJECT
protected:
	//! Текст, который будет изменен при работе данного виджета. Также является входным параметром.
	QString& text_;
	//! Главный менеджер размещения.
	QVBoxLayout* mL_;
	//! Текстовый редактор
	QTextEdit* editor_;
	//! Кнопка "Ок". \details Соединена со слотом pushOk() .
	QPushButton* okButton_;

	//! Конструктор.
	explicit TextDialog(QString& text, QWidget* pwg = Q_NULLPTR);
	//! Деструктор.
	~TextDialog() {}

public:
	//! Статическая функция для создания данного диалогового окна.
	static void createTextDialog(QString& text, QWidget* pwg = Q_NULLPTR){
		TextDialog tdial(text, pwg);
		tdial.exec();
	}

private slots:
	//! Слот, который сохраняет введенный в текстовом редакторе текст во входную переменную text_, закрывает диалоговое окно.
	void pushOk();
};


/*!
 *  \~russian \brief Главный виджет с компонентами для работы симулятора.
 *
 * \~russian Виджет для работы с таблицей "Моделирование" сверхбольшого высокодоступного
 * цифрового каталога данных (Каталога). Явялется составялющей частью копоненты 3d Атласа "Симулятор".
 *
 * Располагет в своей рабочей области модель-представление таблицы "Моделирование". Позволяет запускать
 * работу симулятора с заданнной через конфигурационной файл настройками.
 *
 * Обновляет таблицу периодически - берет данные из БД.
 *
 * Запускает работу симулятора на сервере. Использовать как обычный виджет.
 *
 * Схема использования:
 * \code
 *
 * int owner = 5;
 * SimulatorWidget sw(owner);
 * sw.show();
 *
 * \endcode
 *
 */
class SimulatorWidget: public QWidget {
    Q_OBJECT
private:
	//! ID пользователя. Берется идентификатор с помощью объекта класса userwidget.
	int owner;

	//! Главный менеджер размещения.
	QVBoxLayout*    modellingVLayout;
	//! Менеджер размещения для кнопок Старт, Стоп, Блокнот.
    QHBoxLayout*    buttonsLayout;
	//! Выпадающий список, составленный из названий разных симуляторов в таблице atlas.simulator.
    QComboBox*      modellingComboBox;
	//! Надпись с текстом "Set simulator" (выше выпадающего списка).
	QLabel*         modellingLabel;

	//! Модель таблицы из БД (atlas.modelling) с некоторыми скрытыми строками (каждому пользователю только его строки).
    SimulatorTableModel*    model;
	//! Представление перед пользователем таблицы из БД (atlas.modelling) с некоторыми скрытыми столбцами.
    SimulatorTableView* view;

	//! Кнопка для запуска симулятора. \details Сигнал связан со слотом insertRow(), в котором вызывается метод startExpirement() .
	QPushButton*    startButton;
	//! Кнопка для остановки работы симулятора. \details Сигнал связан со слотом stopExperiment() .
	QPushButton*    stopButton;
	//! Кнопка для вызова диалогового окна с редактором, в который вписывается описание модели. \details Сигнал связан со слотом showTextEdit() .
	QPushButton*	descriptionButton_;
	//! Кнопка для показа виджета с редактором (до таблицы). \details Сигнал связан со слотом shell->show() .
	QPushButton*    editConfigButton;

	//! Виджет текстового редактора вместе с кнопками сохранения, перезагрузки файла и т.п.
    SimpleArtificialShell*  shell;
	//! Строка прогресса для текущего симулятора. Обновляется периодически, уточняя информацию о прогрессе из БД.
	ProgressBar* progressBar_;

	//! Отделитель таблицы и редактора, который позволяет пользователю менять размер одного относительно другого.
	QSplitter* splitter_;

	//! Класс, отнаследованный от QProcess для запуска симулятора.
	SimulatorStarter starter_;

	//! Таймер для обновления всей таблицы. Нужен, чтобы пользователь вовремя заметил slurm_id, т.е. понял, что задача больше не стоит в очереди на суперкомпьютере.
	QTimer* timerForUpdateTable_;
	//! Время в миллисекундах, через которое обновляется таблица и строка прогресса.
	int updateInterval_ {5000};

	//!  Идентификационный номер запущенного процесса. Берется после вставки строки о процессе в БД.
	QString processID_;
	//! Описание модели, которую хотим запустить.
	QString description_;

	//! Функция устанавливает модель-представление для таблицы atlas.modelling из БД. Данная функция должна быть вызвана лишь один раз в конструкторе (желательно первой).
	void initAtlasModellingModelView();
	//! Функция инициализирует кнопки, добавляет их в buttonsLayout, но не добавляет в главный менеджер размещения. Применять в конструкторе после создания shell.
	void initButtons();
	//! Функция берет все симуляторы из базы данных и добавляет их в выпадающий список modellingComboBox.
	void initAllSimulatorsFromDB();

	//! Функция устанавливает фильтр для модели-представления таблицы atlas.modelling из БД. Данная функция должна быть вызвана всякий раз, когда меняется идентификационный номер пользователя (owner).
	void setAtlasModellingRelations();
	//! Функция позволяет получить slurm_id из базы данных для данного процесса. Функция вызывает диалоговое окно с предупреждением, если slurm_id по каким-либо причинам у процесса не установлен. В диалоговом окне можно выбрать либо "да" - попытка прекращения работы процесса в любом случае (не удалит его из очереди на сервере, если он там есть!), либо "нет" - ничего не делает, чтобы дождаться появления slurm_id.
	int getSlurmID();

public:
	//! \~russian Конструктор класса. Принимает идентификационный номер пользователя.
	explicit SimulatorWidget(int ownerIn, QWidget* pwgt = 0);
	//! Деструктор.
	~SimulatorWidget() {}

	//! Функция устанавливает время в миллисекундах, через которое обновляется таблица и строка прогресса. \details Изменяет приватное поле updateInterval_.
	void setUpdateInterval(int updateInterval) { updateInterval_ = updateInterval; }
	//! Функция возвращает время в миллисекундах, через которое обновляется таблица и строка прогресса. \details Читает приватное поле updateInterval_.
	const int& getUpdateInterval() const { return updateInterval_; }

public slots:
	//! \~russian Метод отвечающий за отображение таблицы atlas.modelling, связанной с таблицей atlas.simulator и фильтрующий записи по полю owner. Вызывается классом userwidget, который устанавливает поле owner.   \~russian
	void showModellingtableModel(int idOwner);
	//! \~russian Метод отвечающий за остановку симулятора моделирования. Скрывает строку прогресса, вызывает метод starter_->stopExperiment() .
    void stopExperiment();
	//! \~russian Метод отвечающий за запуск симулятора моделирования. Запускает строку прогресса, таймер, связанный с ней, вызывает метод запуска из starter_.
	void startExpirement();

	//! Функция делает кнопки "старт" и "стоп" доступными для нажатия. \details Изменяет приватные поля startButton и stopButton .
    void enableButtons(const QString text);
	//! Функция показывает диалоговое окно, в которое можно ввести описание для запуска данной модели. \details Изменяет приватное поле description_.
	void showTextEdit();
	//! Функция показывает виджет текстового редактора вместе с кнопками сохранения, перезагрузки файла и т.п. , а также скрывает его, если кнопка нажата второй раз.
	void showSAH();
};

#endif // SIMULATORWIDGET_H

/*@}*/
