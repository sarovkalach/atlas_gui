#include "bookmarkwidget.h"
#include <QDebug>
#include <QHeaderView>

BookmarkWidget::BookmarkWidget(QWidget *pwgt): QWidget(pwgt) {

    bookmarkTableModel = new BookmarkTableModel;
    bookmarkView = new BookmarkView;
#if (QT_VERSION >= QT_VERSION_CHECK(5,7,1))
	bookmarkView->horizontalHeader()->sectionResizeMode( QHeaderView::ResizeToContents );
#else
	bookmarkView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
#endif
    bookmarkLayout = new QVBoxLayout;
    buttonsLayout = new QHBoxLayout;

    bookmarkView->setModel(bookmarkTableModel);
    bookmarkLayout->addWidget(bookmarkView);
    bookmarkView->hideColumn(0);

    insertButton = new QPushButton;
    insertButton->setText(tr("Insert"));
    deleteButton = new QPushButton;
    deleteButton->setText(tr("Delete"));

    buttonsLayout->addWidget(insertButton);
    buttonsLayout->addWidget(deleteButton);

    bookmarkLabelReference = new QLabel;
    bookmarkLabellName = new QLabel;
    bookmarkLabellName->setText(tr("Name"));
    bookmarkLabelReference->setText(tr("Reference"));

    bookmarkLineEditName = new QLineEdit;
    bookmarkLineEditReference = new QLineEdit;

    noteGrid = new QGridLayout;

    noteGrid->addWidget(bookmarkLabellName, 0, 0);
    noteGrid->addWidget(bookmarkLabelReference, 0, 1);
    noteGrid->addWidget(bookmarkLineEditName, 1, 0);
    noteGrid->addWidget(bookmarkLineEditReference, 1, 1);

    bookmarkLayout->addLayout(noteGrid);
    bookmarkLayout->addLayout(buttonsLayout);

    this->setLayout(bookmarkLayout);

    connect( deleteButton, SIGNAL(clicked()), this, SLOT(deleteRow()) );
    connect( insertButton, SIGNAL(clicked()), this, SLOT(insertRow()) );

}




void BookmarkWidget::insertRow() {

    QString reference =  bookmarkLineEditReference->text();
    QString name = bookmarkLineEditName->text();

    query.prepare("INSERT INTO atlas.bookmark (owner, name, reference) VALUES (:owner, :name, :reference)");//
    query.bindValue(":owner", idOwner);
    query.bindValue(":name", name);
    query.bindValue(":reference", reference);
    query.exec();

    showBookmarkTable(idOwner);

}


void BookmarkWidget::deleteRow() {

    bookmarkTableModel->select();
    QModelIndexList indexes = bookmarkView->selectionModel()->selectedIndexes();
    int countRow = indexes.count();

    for( int i = countRow; i > 0; i--) {
           bookmarkTableModel->removeRow( indexes.at(i-1).row(), QModelIndex());
    };

    bookmarkTableModel->submitAll();
    showBookmarkTable(idOwner);
}


void BookmarkWidget::showBookmarkTable(const int &owner) {

    idOwner = owner;
    query.prepare("select name,reference from atlas.bookmark WHERE owner=:owner");//
    query.bindValue(":owner", owner);
    query.exec();

    queryModel.setQuery(query);
    queryModel.setHeaderData(0, Qt::Horizontal,tr("Name"));
    queryModel.setHeaderData(1, Qt::Horizontal,tr("Reference"));

    bookmarkView->setModel(&queryModel);
}




BookmarkWidget::~BookmarkWidget() {
    ;
}
