#include "notewidget.h"
#include <QDebug>


NoteWidget::NoteWidget(QWidget *pwgt): QWidget(pwgt) {

    noteModel = new NoteTableModel;
    noteView = new NoteView;
    noteView->setModel(noteModel);
#if (QT_VERSION >= QT_VERSION_CHECK(5,7,0))
	noteView->horizontalHeader()->sectionResizeMode( QHeaderView::ResizeToContents );
#else
	noteView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
#endif

    noteNameLineEdit = new QLineEdit;
    noteContentLineEdit = new QLineEdit;
    noteContentLabel = new QLabel;
    noteContentLabel->setText(tr("Content"));
    noteNameLabel = new QLabel;
    noteContentLabel->setText(tr("Name"));

    insertButton = new QPushButton;
    insertButton->setText(tr("Insert"));
    deleteButton = new QPushButton;
    deleteButton->setText(tr("Delete"));

    gridLayout = new QGridLayout;
    gridLayout->addWidget(noteView,0,0,1,2);
    gridLayout->addWidget(noteNameLabel,1,0);
    gridLayout->addWidget(noteContentLabel,1,1);
    gridLayout->addWidget(noteNameLineEdit,2,0);
    gridLayout->addWidget(noteContentLineEdit,2,1);
    gridLayout->addWidget(insertButton,3,0);
    gridLayout->addWidget(deleteButton,3,1);

    this->setLayout(gridLayout);

    connect( deleteButton, SIGNAL(clicked()), this, SLOT(deleteRow()) );
    connect( insertButton, SIGNAL(clicked()), this, SLOT(insertRow()) );
}




void NoteWidget::insertRow() {

    QString content =  noteContentLineEdit->text();
    QString name =  noteNameLineEdit->text();
    QDateTime* dateTime = new QDateTime;

    query.prepare("INSERT INTO atlas.note (owner, date, name, content) VALUES (:owner, :date, :content, :name)");//
    query.bindValue(":owner",idOwner);
    query.bindValue(":date", dateTime->currentDateTime().toString("MM.dd.yyyy") );
    query.bindValue(":content", content);
    query.bindValue(":name", name);
    query.exec();
    queryModel.setQuery(query);
    queryModel.submit();

    showNoteTable(idOwner);
}



void NoteWidget::deleteRow() {

    noteModel->select();

    QModelIndexList indexes = noteView->selectionModel()->selectedIndexes();
    int countRow = indexes.count();

    for( int i = countRow; i > 0; i--){
           qDebug()<< "countRow = " << i;

           qDebug() <<" Index beign =  " << indexes.at(i-1).row();
           noteModel->removeRow( indexes.at(i-1).row(), QModelIndex());

    }

    noteModel->submitAll();
    showNoteTable(idOwner);
}

void NoteWidget::showNoteTable(const int &owner) {

    idOwner = owner;
    query.prepare("select name,content,date from atlas.note WHERE owner=:owner");//
    query.bindValue(":owner", owner);
    query.exec();

    queryModel.setQuery(query);
    queryModel.setHeaderData(0, Qt::Horizontal,tr("Name"));
    queryModel.setHeaderData(1, Qt::Horizontal,tr("Date"));
    queryModel.setHeaderData(2, Qt::Horizontal,tr("Content"));

    noteView->setModel(&queryModel);

}
