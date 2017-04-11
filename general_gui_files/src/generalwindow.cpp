#include "../general_gui_files/include/generalwindow.h"

void GeneralWindow::loginUserSlot(bool checkEnterFile){
	loginUser(checkEnterFile);
}

void GeneralWindow::init(){
	menuAccount = new QMenu(tr("&Account"));
	menuBar()->addMenu(menuAccount);

	QAction *loginAction = new QAction("&Login", 0);
	loginAction->setText(tr("Login"));
	loginAction->setShortcut(QKeySequence("SHIFT+L"));
	menuAccount->addAction(loginAction);

	menuHelp = new QMenu(tr("Help"));
	menuBar()->addMenu(menuHelp);

	owner_ = userRecord_.tryToGetOwner(true);				//по хорошему, этод код надо бы запихать в метод loginUserSlot(), но т.к. там вызывается виртуальная функция, то в конструкторе ее вызывать уже нельзя.

	connect(loginAction, SIGNAL(triggered()), SLOT(loginUserSlot()));
}
