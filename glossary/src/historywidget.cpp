#include "historywidget.h"
#include <QModelIndexList>
#include <QDebug>
#include <QDateTime>

HistoryWidget::HistoryWidget(QWidget *pwgt): QWidget(pwgt) {

    historyModel = new HistoryTableModel;
    historyView = new HistoryView;
    historyLayout = new QVBoxLayout;
    buttonsLayout = new QHBoxLayout;

    historyView->setModel(historyModel);
    historyLayout->addWidget(historyView);

    insertButton = new QPushButton;
    insertButton->setText(tr("Insert"));
    deleteButton = new QPushButton;
    deleteButton->setText(tr("Delete"));

    buttonsLayout->addWidget(deleteButton);
    historyLayout->addLayout(buttonsLayout);
    this->setLayout(historyLayout);

    connect( deleteButton, SIGNAL(clicked()), this, SLOT(deleteRow()) );
}



void HistoryWidget::deleteRow() {

    historyModel->select();
    QModelIndexList indexes = historyView->selectionModel()->selectedIndexes();
    int countRow = indexes.count();
    qDebug() << countRow;

    for( int i = countRow; i > 0; i--){
           historyModel->removeRow( indexes.at(i-1).row(), QModelIndex());
           historyModel->submit();
    }

    showHistoryTable(idOwner_);
}


void HistoryWidget::showHistoryTable(const int &owner) {

    idOwner_ = owner;
    query.prepare("select date,request from atlas.history WHERE owner=:owner");//
    query.bindValue(":owner", owner);
    query.exec();

    queryModel.setQuery(query);
    historyView->setModel(&queryModel);
    queryModel.setQuery(query);
    queryModel.setHeaderData(1,Qt::Horizontal,tr("Request"));
    queryModel.setHeaderData(0,Qt::Horizontal,tr("Date"));
}


void HistoryWidget::updateHistoryTable() {

    query.prepare("select date,request from atlas.history WHERE owner=:owner");//
    query.bindValue(":owner", idOwner_);
    query.exec();
    queryModel.setQuery(query);
    historyView->setModel(&queryModel);

}

void HistoryWidget::addRow(int owner, QString& request) {

    idOwner_ = owner;
    QDateTime* dateTime = new QDateTime;

    query.prepare("INSERT INTO atlas.history (owner, date, request) VALUES (:owner, :date, :request)");//
    query.bindValue(":owner",idOwner_);
    query.bindValue(":date", dateTime->currentDateTime().toString("MM.dd.yyyy") );
    query.bindValue(":request", request);
    query.exec();
    queryModel.setQuery(query);
    queryModel.submit();

    showHistoryTable(idOwner_);
}

HistoryWidget::~HistoryWidget() {
    ;
}
