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


/*! \brief Кратко.
 *
 * Длинно.
 *
 * Схема использования:
 * \code
 *
 * \endcode
 *
 */
class ProgressBar : public QWidget{
	Q_OBJECT
private:
	//!
	const QString& id_;
	//!
	QProgressBar* bar_ {Q_NULLPTR};
	//!
	QTimer* timer_ {Q_NULLPTR};
	//!
	int timerInterval_ {5000};
	//!
	QHBoxLayout* mL_ {Q_NULLPTR};

	//!
	void init();

public:
	//!
	explicit ProgressBar(const QString& id, QWidget* pwg = Q_NULLPTR);
	//!
	~ProgressBar() {}

	//!
	void setTimerInterval(int ti) { timerInterval_ = ti; timer_->setInterval(timerInterval_); }
	//!
	const int& getTimerInterval() const { return timerInterval_; }
	//!
	const QString& getId() const { return id_; }
	//!
	int getIntId() const;

public slots:
	//!
	void updateProgressBar();
	//!
	void initTimer();
	//!
	void deleteTimer();
};

#endif // PROGRESSBAR_16032017_GASPARYANMOSES

/*@}*/
