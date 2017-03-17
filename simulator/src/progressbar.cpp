#include "progressbar.h"

void ProgressBar::initTimer(){
	timer_->start(timerInterval_);
	show();
}
void ProgressBar::deleteTimer(){
	timer_->stop();
	hide();
}

int ProgressBar::getIntId() const{
	std::stringstream vss;
	vss.str(id_.toStdString());
	int res;
	vss>>res;
	return res;
}

void ProgressBar::updateProgressBar(){
	int progressBarValue = 0;
	QSqlQuery query;
	QSqlQueryModel queryModel;
	query.prepare("SELECT progress FROM atlas.modelling WHERE id=" + id_);
	query.exec();
	queryModel.setQuery(query);

	progressBarValue = queryModel.data(queryModel.index(0,0)).toInt();

	bar_->setValue(progressBarValue);
}

void ProgressBar::init(){
	bar_ = new QProgressBar;
	bar_->setMinimum(0);
	bar_->setMaximum(100);
	bar_->setValue(0);

	timer_ = new QTimer;
	timer_->setInterval(timerInterval_);

	mL_ = new QHBoxLayout;
	mL_->addWidget(bar_);
	setLayout(mL_);

	connect(timer_, SIGNAL(timeout()), this, SLOT(updateProgressBar()));

	hide();
}

ProgressBar::ProgressBar(const QString& id, QWidget *pwg) : QWidget(pwg), id_(id){
	init();
}
