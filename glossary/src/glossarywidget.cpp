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

}


void GlossaryWidget::addHistoryRow() {

    QDateTime* dateTime = new QDateTime;
    QString searhRecord;
    HistoryWidget* historyWidget = new HistoryWidget;

    int lastRow = historyWidget->historyModel->rowCount();
    searhRecord = this->glossFilterNotion->text();

    historyWidget->historyModel->insertRow(lastRow);
    historyWidget->historyModel->setData(historyWidget->historyModel->index(0,1), 1); // ID from users
    historyWidget->historyModel->setData(historyWidget->historyModel->index(0,2), dateTime->currentDateTime().toString("MM.dd.yyyy")); //insert current date
    historyWidget->historyModel->setData(historyWidget->historyModel->index(0,3), searhRecord);  //insert search position
    qDebug() << historyWidget->historyModel->submitAll();
    qDebug() << historyWidget->historyModel->lastError().text();

    connect(this, SIGNAL(updateHistoryTable()), this->parentWidget(), SLOT(updateTableWidget()));
    emit updateHistoryTable();

}

void GlossaryWidget::setFilterNotion(const QString& filter) {
    glossModel->setFilter("notion LIKE '%"+filter+"%'");
}




GlossaryWidget::~GlossaryWidget() {

}
