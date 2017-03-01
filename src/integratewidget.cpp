#include "integratewidget.h"

#include <QDebug>

IntegrateWidget::IntegrateWidget(QWidget* pwgt) : QWidget(pwgt)
{
    model = new IntegrateTableModel;
    view = new IntegrateTableView;
    view->setModel(model);

    owner = 1;                      // ID from StartWindow
    showIntegrateTableModel(owner);

    modellingLabel = new QLabel;
    modellingVLayout = new QVBoxLayout;
    modellingHLayout = new QHBoxLayout;
    buttonsLayout = new QHBoxLayout;
    modellingComboBox = new QComboBox;
    modellingLineEdit = new  QLineEdit;
    loadConfigrButton = new QPushButton;
    editConfigButton = new QPushButton;
    startButton = new QPushButton;
    stopButton = new QPushButton;
    deleteButton = new QPushButton;

    simulatorTable = new QSqlRelationalTableModel;
    simulatorTable->setTable("atlas.simulator");
    simulatorTable->select();

    modellingLabel->setText(tr("Set simulator"));
    loadConfigrButton->setText(tr("Load config"));
    editConfigButton->setText(tr("Notepad"));
    startButton->setText(tr("Start"));
    stopButton->setText(tr("Stop"));
    deleteButton->setText(tr("Delete"));

    for (int i=0; i < simulatorTable->rowCount(); i++ )
         modellingComboBox->addItem(simulatorTable->data(simulatorTable->index(i,2)).toString());

    modellingVLayout->addWidget(modellingLabel);
    modellingVLayout->addWidget(modellingComboBox);
    modellingHLayout->addWidget(modellingLineEdit);
    modellingHLayout->addWidget(loadConfigrButton);
    modellingVLayout->addLayout(modellingHLayout);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(editConfigButton);
    modellingVLayout->addLayout(buttonsLayout);
    modellingVLayout->addWidget(view);
    modellingVLayout->addWidget(notepad);
    notepad->hide();

    connect(loadConfigrButton, SIGNAL(clicked(bool)), this, SLOT(openConfigFile()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startExpirement()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(insertRow()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
    connect(editConfigButton,SIGNAL(clicked(bool)),this,SLOT(openNotepad()));
    this->setLayout(modellingVLayout);
}



void IntegrateWidget::updateWidget() {
    this->update();
    this->show();
}


void IntegrateWidget::startExpirement() {
    qDebug() << "Start Experiment";
    //QProcess* process = new QProcess;
    //process->start("/home/kalach/Qt/GUI/GUI");
}



void IntegrateWidget::insertRow()
{
    QDateTime* dateTime = new QDateTime;
    id = getSimulatorID(modellingComboBox->currentText());
    qDebug()<< "Simulator ID = " << id;

    query.prepare("INSERT INTO atlas.integrate (owner, date, spp, comments, brain) VALUES (:owner, :date, :spp, :comments, :brain)");//
    query.bindValue(":owner", owner);
    query.bindValue(":date", dateTime->currentDateTime().toString("MM.dd.yyyy"));
    //query.bindValue(":beg_file", modellingLineEdit->text());
    query.bindValue(":comments", "test");
    query.bindValue(":brain", "brain");
    query.bindValue(":spp", modellingLineEdit->text());
    query.exec();
    qDebug()<< query.lastError();
    showIntegrateTableModel(owner);
}


void IntegrateWidget::showIntegrateTableModel(const int idOwner)
{
    owner = idOwner;
    //model->setRelation(3, QSqlRelation("atlas.simulator", "id", "name"));
    QString filter = QString("owner= %1 ").arg(owner);
    model->setFilter(filter);

    //model->setHeaderData(3, Qt::Horizontal,tr("Model Name"));
    //model->setHeaderData(2, Qt::Horizontal,tr("Date"));
    //model->setHeaderData(4, Qt::Horizontal,tr("Config"));
    //model->select();

    view->setModel(model);
    //view->hideColumn(0);
    //view->hideColumn(1);

    this->update();
}


int IntegrateWidget::getSimulatorID(QString simulatorName)
{
    /*
    query.prepare("select * from atlas.integrate WHERE reference=:reference ");//
    query.bindValue(":reference", simulatorName);
    query.exec();
    queryModel.setQuery(query);

    return queryModel.data(queryModel.index(0,0)).toInt();
    */
}


void IntegrateWidget::openConfigFile()
{
    QFont font("", 0);
    QFontMetrics fm(font);

    modellingFileDialog = new QFileDialog;
    QString fileDir = modellingFileDialog->getOpenFileName(this, "Select  file", QDir::homePath());
    modellingLineEdit->setText(fileDir);

    if (fileDir != "") {
        int pixelsWide = fm.width(fileDir); // automatic resize QlineEdit
        int pixelsHigh = fm.height();

        modellingLineEdit->setFixedSize(pixelsWide, pixelsHigh + 1);
    }
    connect(this,SIGNAL(setFile(QString)), notepad,SLOT(openFile(QString)));
    if (fileDir != "")
        emit setFile(fileDir);
}


void IntegrateWidget::setFilenamefromNotepad(QString fielname) {
    modellingLineEdit->setText(fielname);
}

void IntegrateWidget::removeRow()
{
    qDebug() << "Row removed";
    QModelIndexList indexes = view->selectionModel()->selectedIndexes();

    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--) {
           model->removeRow( indexes.at(i-1).row(), QModelIndex());
    };
    model->submitAll();
    showIntegrateTableModel(owner);
}


void IntegrateWidget::openNotepad() {
    notepad->show();
}
