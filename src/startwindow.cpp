#include "startwindow.h"
#include "QDebug"

StartWindow::StartWindow(QWidget *pwgt) : QDialog(pwgt) {

    translator = new QTranslator;
    languageComboBox = new QComboBox();
    languageComboBox->addItem("English");
    languageComboBox->addItem("Russian");

    languageLabel = new QLabel;
    languageLabel->setText("Choose Languge");

    startLayout = new QVBoxLayout;
    startLayout->addWidget(languageLabel);
    startLayout->addWidget(languageComboBox);

    okButton = new QPushButton;
    okButton->setText("Ok");
    cancelButton = new QPushButton;
    cancelButton->setText("Cancel");

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    startLayout->addLayout(buttonsLayout);
    this->setLayout(startLayout);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(startUp()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));

}


void StartWindow::setApp(QApplication *applictaion) {
    app = applictaion;
}


void StartWindow::startUp() {

    language = languageComboBox->currentText();
    QFile file("/home/kalach/Qt/GUI/myfile.txt");
    setLanguage(language);
    main = new MainWindow;
    //main->hide();
    this->close();

    if ( file.exists() ) {
        QString login;
        QString password;;
        QString hash;

        if ( !file.open(QIODevice::ReadOnly) ) {
            qDebug() << "Ошибка при открытии файла";
        }

        else  {

            // remove "\n" from each word
            login = file.readLine();
            login = login.remove(login.size()-1, 1);

            password = file.readLine();
            password = password.remove(password.size()-1, 1);

            hash = file.readLine();
            hash = hash.remove(hash.size()-1, 1);

            qDebug() << login << "|" << password << endl;
            main->showWidget(hash, login);
        }

    }

   else {
        main->loginUser();
   }
}
void StartWindow::setLanguage(const QString lang) {
    if (lang == "Russian")
        translator->load("/home/kalach/Qt/GUI/languages/main_ru.qm");
    else
        translator->load("/home/kalach/Qt/GUI/languages/main_en.qm");

    app->installTranslator(translator);

}
