#include "integratormainwindow.h"

void IntegratorMainWindow::loginUser(bool checkEnterFile){
	owner_ = userRecord_.tryToGetOwner(checkEnterFile);
	integratorWidget_->showIntegratetableModel(owner_);
}

IntegratorMainWindow::IntegratorMainWindow(QWidget *pwg) : GeneralWindow(pwg){
	integratorWidget_ = new IntegratorWidget(owner_);
	setCentralWidget(integratorWidget_);

	integratorWidget_->showIntegratetableModel(owner_);

	setGeometry(0,0,1024,768);
}
