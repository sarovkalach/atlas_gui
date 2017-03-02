#include "simulatorwidget.h"
#include <QDebug>

SimulatorWidget::SimulatorWidget(QWidget* pwgt) : QWidget(pwgt)
{
    model = new SimulatorTableModel;
    view = new SimulatorTableView;
    view->setModel(model);
    view->setEditTriggers(QTableView::NoEditTriggers);  // read only view

    owner = 1;                      // ID from StartWindow

    simulator = new SimulatorStarter;
    showModellingtableModel(owner);
    shell = new SimpleArtificialShell;
    modellingLabel = new QLabel;

    modellingVLayout = new QVBoxLayout;
    modellingHLayout = new QHBoxLayout;
    buttonsLayout = new QHBoxLayout;
    modellingComboBox = new QComboBox;
    editConfigButton = new QPushButton;
    startButton = new QPushButton;
    startButton->setDisabled(true);
    stopButton = new QPushButton;
    stopButton->setDisabled(true);
    simulatorTable = new QSqlRelationalTableModel;
    simulatorTable->setTable("atlas.simulator");
    simulatorTable->select();

    modellingLabel->setText(tr("Set simulator"));
    editConfigButton->setText(tr("Notepad"));
    startButton->setText(tr("Start"));
    stopButton->setText(tr("Stop"));

    for (int i=0; i < simulatorTable->rowCount(); i++ )
         modellingComboBox->addItem(simulatorTable->data(simulatorTable->index(i,2)).toString());

    modellingVLayout->addWidget(modellingLabel);
    modellingVLayout->addWidget(modellingComboBox);

    modellingVLayout->addLayout(modellingHLayout);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(editConfigButton);
    modellingVLayout->addLayout(buttonsLayout);
    modellingVLayout->addWidget(shell);
    modellingVLayout->addWidget(view);
    shell->hide();
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startExpirement()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(insertRow()));
    connect(editConfigButton,SIGNAL(clicked(bool)), this,SLOT(openNotepad()));
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
	query.bindValue(":beg_file", "her");
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

void SimulatorWidget::openNotepad() {
    shell->show();
}


void SimulatorWidget::enableButtons(QString text) {
    qDebug() << "Fuck you";
}
