#include "simulatorwidget.h"
#include <QDebug>

SimulatorWidget::SimulatorWidget(QWidget* pwgt) : QWidget(pwgt)
{
    model = new SimulatorTableModel;
    view = new SimulatorTableView;
    view->setModel(model);

    owner = 1;                      // ID from StartWindow

    simulator = new SimulatorStarter;
    showModellingtableModel(owner);
    notepad = new NotepadWidget;
    notepad->hide();

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
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopExperiment()));
    this->setLayout(modellingVLayout);

}



void SimulatorWidget::updateWidget() {
    this->update();
    this->show();
}


void SimulatorWidget::startExpirement() {
    qDebug() << "Start Experiment";
    simulator->start("/home/kalach/Qt/sleep");
}

void SimulatorWidget::stopExperiment() {
    qDebug() << "Stop Experiment";
    simulator->close();
    showModellingtableModel(owner);
}

void SimulatorWidget::insertRow()
{
    QDateTime* dateTime = new QDateTime;
    id = getSimulatorID(modellingComboBox->currentText());
    qDebug()<< "Simulator ID = " << id;

    query.prepare("INSERT INTO atlas.modelling (owner, beg_date, simulator_id, beg_file, description, progress) VALUES (:owner, :beg_date, :simulator_id, :beg_file, :description, :progress)");//
    query.bindValue(":owner", owner);
    query.bindValue(":beg_date", dateTime->currentDateTime().toString("MM.dd.yyyy"));
    query.bindValue(":beg_file", modellingLineEdit->text());
    query.bindValue(":description", "test");
    query.bindValue(":progress", 1);
    query.bindValue(":simulator_id", id);
    query.exec();

    showModellingtableModel(owner);
}


void SimulatorWidget::showModellingtableModel(const int idOwner)
{
    owner = idOwner;
    model->setRelation(3, QSqlRelation("atlas.simulator", "id", "name"));
    QString filter = QString("owner= %1 ").arg(owner);
    model->setFilter(filter);

    model->setHeaderData(3, Qt::Horizontal,tr("Model Name"));
    model->setHeaderData(2, Qt::Horizontal,tr("Date"));
    model->setHeaderData(4, Qt::Horizontal,tr("Config"));

    model->select();

    view->setModel(model);
    //view->hideColumn(0);
    //view->hideColumn(1);

    this->update();
}


int SimulatorWidget::getSimulatorID(QString simulatorName)
{
    query.prepare("select * from atlas.simulator WHERE reference=:reference ");//
    query.bindValue(":reference", simulatorName);
    query.exec();
    queryModel.setQuery(query);

    return queryModel.data(queryModel.index(0,0)).toInt();
}


void SimulatorWidget::openConfigFile()
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


void SimulatorWidget::setFilenamefromNotepad(QString fielname) {
    modellingLineEdit->setText(fielname);
}

void SimulatorWidget::removeRow()
{
    qDebug() << "Row removed";
    QModelIndexList indexes = view->selectionModel()->selectedIndexes();

    int countRow = indexes.count();
    for( int i = countRow; i > 0; i--) {
           model->removeRow( indexes.at(i-1).row(), QModelIndex());
    };
    model->submitAll();
    showModellingtableModel(owner);
}


void SimulatorWidget::openNotepad() {
    notepad->show();
}


