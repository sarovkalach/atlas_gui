#include "glossarywidget.h"
#include <QDebug>


GlossaryWidget::GlossaryWidget(QWidget *pwgt) : QWidget(pwgt) {

    glossaryLabel = new QLabel(tr("Filter"));
    glossVBoxLayout = new QVBoxLayout;
    glossTableview = new GlossaryView;

    glossModel = new GlossaryTableModel;
    glossTableview->setModel(glossModel);
#if (QT_VERSION >= QT_VERSION_CHECK(5,7,0))
	glossTableview->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
	glossTableview->horizontalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
#else
	glossTableview->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	glossTableview->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    glossTableview->resizeColumnsToContents();

    glossTableview->hideColumn(0);
    QToolBar *toolFilter = new QToolBar(tr("Filter"));

    QWidgetAction *notionAction = new QWidgetAction(this);
    glossFilterNotion = new QLineEdit(tr(""));
    glossFilterNotion->setDisabled(0);  // lock widget

    notionAction->setDefaultWidget(glossFilterNotion);
    toolFilter->addAction(notionAction);

    glossVBoxLayout->addWidget(glossTableview);
    glossVBoxLayout->addWidget(glossaryLabel);
    glossVBoxLayout->addWidget(toolFilter);

    this->setLayout(glossVBoxLayout);

    connect(glossFilterNotion, SIGNAL(textChanged(const QString&)), this, SLOT(setFilterNotion(const QString&)));
    connect(glossFilterNotion, SIGNAL(returnPressed()), this, SLOT(addHistoryRow()));

    historyWidget = new HistoryWidget;
    connect(this, SIGNAL(addRowinHistoryTable(int,QString&)), historyWidget, SLOT(addRow(int, QString&)));
}


void GlossaryWidget::addHistoryRow() {

    QString searhRecord;

    int lastRow = historyWidget->historyModel->rowCount();
    searhRecord = this->glossFilterNotion->text();

    qDebug() << "Last row = "<< lastRow;
    emit addRowinHistoryTable(owner_, searhRecord);
    connect(this, SIGNAL(updateHistoryTable()), this->parentWidget(), SLOT(updateTableWidget()));
    emit updateHistoryTable();
}

void GlossaryWidget::setFilterNotion(const QString& filter) {
    glossModel->setFilter("notion LIKE '%"+filter+"%'");
}


void GlossaryWidget::setOwner(int id) {
    owner_ = id;
}

GlossaryWidget::~GlossaryWidget() {

}
