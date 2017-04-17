#include <QApplication>
#include <QMessageBox>

#include "../general_gui_files/include/startwindow.h"
#include "simulatormainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	int res = StartWindow::createStartWindow(&app);
	if (res == QDialog::Rejected){
		app.closeAllWindows();
		app.exit(0);
		return 0;
	}

	connectToTheDataBase(app);

	SimulatorMainWindow mw;
	mw.show();

    return app.exec();
}
