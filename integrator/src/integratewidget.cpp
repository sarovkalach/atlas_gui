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
    editConfigButton = new QPushButton;
    startButton = new QPushButton;
    stopButton = new QPushButton;
    deleteButton = new QPushButton;
    shell = new SimpleArtificialShell;
    simulatorTable = new QSqlRelationalTableModel;
    simulatorTable->setTable("atlas.simulator");
    simulatorTable->select();

    modellingLabel->setText(tr("Set simulator"));
    editConfigButton->setText(tr("Notepad"));
    startButton->setText(tr("Start"));
    startButton->setDisabled(true);
    stopButton->setText(tr("Stop"));
    stopButton->setDisabled(true);

    deleteButton->setText(tr("Delete"));

    for (int i=0; i < simulatorTable->rowCount(); i++ )
         modellingComboBox->addItem(simulatorTable->data(simulatorTable->index(i,2)).toString());

    modellingVLayout->addWidget(modellingLabel);
    modellingVLayout->addWidget(modellingComboBox);
    modellingVLayout->addLayout(modellingHLayout);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(editConfigButton);
    modellingVLayout->addLayout(buttonsLayout);
    modellingVLayout->addWidget(shell);
    modellingVLayout->addWidget(view);
    shell->hide();
    shell->setParent(this);

    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startExpirement()));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(insertRow()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(removeRow()));
    connect(editConfigButton,SIGNAL(clicked(bool)),this, SLOT(openNotepad()));
    connect(shell->get_inFileLineEditor(), SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));

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
    query.prepare("INSERT INTO atlas.integrate (owner, date, spp, comments, brain) VALUES (:owner, :date, :spp, :comments, :brain)");//
    query.bindValue(":owner", owner);
    query.bindValue(":date", dateTime->currentDateTime().toString("MM.dd.yyyy"));
    query.bindValue(":comments", "test");
    query.bindValue(":brain", "brain");
    query.bindValue(":spp", shell->get_inFileLineEditor()->text());
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

    model->setHeaderData(2, Qt::Horizontal, tr("Date"));
    model->setHeaderData(3, Qt::Horizontal, tr("Brain"));
    model->setHeaderData(4, Qt::Horizontal, tr("IIP"));
    model->setHeaderData(5, Qt::Horizontal, tr("EDP"));
    model->setHeaderData(6, Qt::Horizontal, tr("SPP"));
    model->setHeaderData(7, Qt::Horizontal, tr("RP"));
    model->setHeaderData(8, Qt::Horizontal, tr("Comments"));

    //model->select();

    view->setModel(model);
    view->hideColumn(0);
    view->hideColumn(1);

    this->update();
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
    shell->show();
}

void IntegrateWidget::enableButtons(const QString text) {
    startButton->setEnabled(true);
    stopButton->setEnabled(true);
}
