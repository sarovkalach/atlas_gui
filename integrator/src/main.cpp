#include <QApplication>

#include "../general_gui_files/include/startwindow.h"
#include "integratormainwindow.h"

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

	IntegratorMainWindow mw;
	mw.show();

	return app.exec();
}
