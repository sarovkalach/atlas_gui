#include "simulatormainwindow.h"

void SimulatorMainWindow::loginUser(bool checkEnterFile){
	owner_ = userRecord_.tryToGetOwner(checkEnterFile);
	simulatorWidget_->showModellingtableModel(owner_);
}

SimulatorMainWindow::SimulatorMainWindow(QWidget *pwgt) : GeneralWindow(pwgt){
	simulatorWidget_ = new SimulatorWidget(owner_);
	setCentralWidget(simulatorWidget_);
	simulatorWidget_->setParent(this);

	simulatorWidget_->showModellingtableModel(owner_);				//по хорошему, надо бы вызвать loginUser(), но т.к. это виртуальная функция, то в конструкторе ее вызывать не стоит. Т.к. кода не много, то выделять отдельную невиртуальную функцию, которая бы вызывалась в виртуальной, (а в конструкторе использовалась бы не виртуальная) не нужно.

	setGeometry(0, 0, 1024, 768);
}
