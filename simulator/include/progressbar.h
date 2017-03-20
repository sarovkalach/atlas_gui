/** @addtogroup oscillators
 * @{*/

/** @file */

#ifndef PROGRESSBAR_16032017_GASPARYANMOSES
#define PROGRESSBAR_16032017_GASPARYANMOSES

#include <QProgressBar>
#include <QTimer>
#include <QWidget>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <sstream>


/*! \brief Виджет строки прогресса.
 *
 * Виджет строки прогресса. Прогресс берется из БД периодически (с помощью таймера).
 * Данный виджет только читает базу данных, но ничего туда не
 * записывает.
 * Используется другими виджетами. Имя таблицы, к которой обращается данный виджет
 * не может быть изменено.
 *
 * Схема использования:
 * \code
 *  QVBoxLayout* modellingVLayout = new QVBoxLayout;
 *
 *  //...
 *
 * 	progressBar_ = new ProgressBar(processID_);
 *	modellingVLayout->addWidget(progressBar_);
 *  progressBar_->setTimerInterval(5000);
 *	progressBar_->initTimer();
 *
 * \endcode
 *
 */
class ProgressBar : public QWidget{
	Q_OBJECT
private:
	//! Идентификационный номер записи в БД таблицы atlas.modelling .
	const QString& id_;
	//! Виджет строки прогресса.
	QProgressBar* bar_ {Q_NULLPTR};
	//! Таймер. Позволяет делать некоторое событие периодически.
	QTimer* timer_ {Q_NULLPTR};
	//! Интервал для таймера в миллисекундах.
	int timerInterval_ {5000};
	//! Главный менеджер размещения для данного виджета.
	QHBoxLayout* mL_ {Q_NULLPTR};

	//! Функция инициализации. Должна быть вызвана лишь один раз в конструкторе.
	void init();

public:
	//! Конструктор.
	explicit ProgressBar(const QString& id, QWidget* pwg = Q_NULLPTR);
	//! Деструктор.
	~ProgressBar() {}

	//! Функция устанавливает интервал для таймера в миллисекундах. \details Изменяет приватное поле timerInterval_ и timer_.
	void setTimerInterval(int ti) { timerInterval_ = ti; timer_->setInterval(timerInterval_); }
	//! Функция возвращает интервал для таймера в миллисекундах. \details Читает приватное поле timerInterval_.
	const int& getTimerInterval() const { return timerInterval_; }
	//! Функция возвращает идентификационный номер записи в БД таблицы atlas.modelling. Перегруженная функция, версия для строк. \details Читает приватное поле id_.
	const QString& getId() const { return id_; }
	//! Функция возвращает идентификационный номер записи в БД таблицы atlas.modelling. Перегруженная функция, версия для целых чисел. \details Читает приватное поле id_.
	int getIntId() const;

public slots:
	//! Функция запрашивает прогресс из таблицы в БД atlas.modelling и выводит соответствующее значение в строку прогресса. Данная функция вызывается таймером периодически.
	void updateProgressBar();
	//! Функция запускает таймер, показывает виджет. Данный слот может быть использован другими виджетами. Не стоит вызывать метод show() самостоятельно.
	void initTimer();
	//! Функция останавливает работу таймера, скрывает данный виджет. Не стоит вызывать метод hide() самостоятельно.
	void deleteTimer();
};

#endif // PROGRESSBAR_16032017_GASPARYANMOSES

/*@}*/
