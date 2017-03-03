#include "glossary.h"



Glossary::Glossary(QWidget *pwgt) : QWidget(pwgt) {

    totalGrid = new QGridLayout;

    historyWidget = new HistoryWidget;
    noteWidget = new NoteWidget;
    historyWidget = new HistoryWidget;
    glossaryWidget = new GlossaryWidget;
    bookmarkWidget = new BookmarkWidget;

    noteWidget->showNoteTable(idOwner);                //requests for DB
    historyWidget->showHistoryTable(idOwner);
    bookmarkWidget->showBookmarkTable(idOwner);

    totalGrid->addWidget(bookmarkWidget,0,0,1,2);
    totalGrid->addWidget(noteWidget,1,0,1,2);
    totalGrid->addWidget(glossaryWidget,0,2,2,1);
    totalGrid->addWidget(historyWidget,0,4,2,2);

    this->setLayout(totalGrid);

}

void Glossary::closeEvent(QCloseEvent *event) {
     event->accept();
     delete this;
}

void Glossary::updateWidget() {
    this->update();
    this->show();
}

void Glossary::updateTableWidget() {
    this->historyWidget->updateHistoryTable();
}

Glossary::~Glossary() {
    ;
}
