#include "notepadwidget.h"
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QDebug>


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    paradigmaNeursFormat.setFontItalic(true);
    paradigmaNeursFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("#Nneurs");
    rule.format = paradigmaNeursFormat;
    highlightingRules.append(rule);

    paradigmaNeursParamsFormat.setFontItalic(true);
    paradigmaNeursParamsFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("#Nneurs params");
    rule.format = paradigmaNeursParamsFormat;
    highlightingRules.append(rule);


    floatFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("\\d+\\.\\d+");
    rule.format = floatFormat;
    highlightingRules.append(rule);

    integerFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegExp("^\\d+$");
    rule.format = integerFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
   }

void Highlighter::highlightBlock(const QString &text) {

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}


NotepadWidget::NotepadWidget(QWidget *pwgt) : QWidget(pwgt)
{
    txtField = new QTextEdit;

    openButton = new QPushButton();
    openButton->setText(tr("Open"));
    saveButton = new QPushButton;
    saveButton->setText(tr("Save"));
    saveButton->setEnabled(false);
    saveAsButton = new QPushButton();
    saveAsButton->setText(tr("Save As"));
    saveAsButton->setEnabled(false);

    exportButton = new QPushButton;
    exportButton->setText(tr("Export"));
    closeButton = new  QPushButton;
    closeButton->setText(tr("Close"));
    newFileButton = new QPushButton;
    newFileButton->setText(tr("New File"));

    buttonsLeftLayout = new QVBoxLayout;
    buttonsRightLayout = new QVBoxLayout;
    buttonsLeftLayout->addWidget(openButton);
    buttonsLeftLayout->addWidget(saveButton);
    buttonsLeftLayout->addWidget(saveAsButton);
    buttonsLeftLayout->addWidget(closeButton);


    buttonsRightLayout->addWidget(exportButton);
    buttonsRightLayout->addWidget(newFileButton);

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLeftLayout);
    mainLayout->addWidget(txtField);
    mainLayout->addLayout(buttonsRightLayout);

    this->setLayout(mainLayout);

    connect(openButton, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(saveFile()));
    connect(saveAsButton, SIGNAL(clicked(bool)), this, SLOT(saveFileAs()));

    connect(exportButton,SIGNAL(clicked(bool)),this, SLOT(exportFile()));
    connect(closeButton, SIGNAL(clicked(bool)),this, SLOT(close()));
    connect(newFileButton,SIGNAL(clicked(bool)),this,SLOT(newFileDialog()));
    connect(txtField,SIGNAL(textChanged()),this,SLOT(enableSaveButton()));

    //QRegExp reg("#[A-Z]+");            //  paradigms expression
    //QRegExp reg("^\\d+$");           //  integer  valuse expression
    //QRegExp reg("\\d+.\\d+");        //  floating point values regexpresion
    //QString str = "#Work out";
    //QString str = "1";
    //qDebug()<< (str.contains(reg) > 0);
}


void NotepadWidget::openFile()
{
    if (fileName!= "") {
        userDialog = new UserDialog;
        userDialog->dialogLabel->setText(tr("Are you sure open new file ?"));
        userDialog->dialogLineEdit->hide();
        connect(userDialog->okButton,SIGNAL(clicked(bool)),userDialog,SLOT(close()));
        connect(userDialog->okButton,SIGNAL(clicked(bool)),this,SLOT(openFileSlot()));
        connect(userDialog->cancelButton,SIGNAL(clicked(bool)),userDialog,SLOT(close()));
        userDialog->show();
    }
    else
    {
        openFileSlot();
    }

}

void NotepadWidget::openFileSlot()
{
    openFileDialog = new QFileDialog;
    fileName = openFileDialog->getOpenFileName(this, "Select  file", QDir::homePath());
    this->readFile();
}

void NotepadWidget::openFile(QString filename) {

    fileName = filename;
    this->readFile();
    this->show();
}


void NotepadWidget::readFile()
{
    file.setFileName(fileName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    txtField->setText(stream.readAll());
    file.close();
    highlighter = new Highlighter(txtField->document());
}



void NotepadWidget::saveFile()
{
    if(fileName!="")
    {
        saveFileSlot();
    }

    else
    {
        userDialog = new UserDialog;
        userDialog->dialogLabel->setText(tr("Are you sure save file ?"));
        userDialog->dialogLineEdit->hide();
        connect(userDialog->okButton,SIGNAL(clicked(bool)),userDialog,SLOT(close()));
        connect(userDialog->okButton,SIGNAL(clicked(bool)),this,SLOT(newFile()));
        connect(userDialog->cancelButton,SIGNAL(clicked(bool)),userDialog,SLOT(close()));
        userDialog->show();
    }
    qDebug()<< "File Saved";
}


void NotepadWidget::saveFileSlot()
{
    QTextStream out(&file);
    file.setFileName(fileName);
    file.open(QFile::WriteOnly | QFile::Text);
    out<< txtField->toPlainText();
    file.close();
}


void NotepadWidget::saveFileAs()
{
    userDialog = new UserDialog;
    userDialog->dialogLabel->setText(tr("Set File Name"));

    connect(userDialog->okButton,SIGNAL(clicked(bool)),this,SLOT(createNewFile()));
    userDialog->show();
}

void NotepadWidget::exportFile()
{
    if (fileName != "") {
        connect(this,SIGNAL(setFileName(QString)),parentWidget(),SLOT(setFilenamefromNotepad(QString)));
        emit setFileName(fileName);
         qDebug()<< "Export File";
    }
    else
        qDebug()<< "Nothing to export";
}


void NotepadWidget::newFileDialog()
{
    qDebug()<< "New File Dialog";

    userDialog = new UserDialog;
    if ( fileName != "") {
        userDialog->dialogLabel->setText(tr("Are you sure create new file ?"));
        userDialog->dialogLineEdit->hide();
        connect(userDialog->okButton,SIGNAL(clicked(bool)),this,SLOT(newFile()));
        userDialog->show();
    }
}



void NotepadWidget::newFile()
{
    userDialog = new UserDialog;
    userDialog->dialogLabel->setText(tr("Set File Name"));

    connect(userDialog->okButton,SIGNAL(clicked(bool)),this,SLOT(createNewFile()));
    userDialog->show();
    txtField->clear();
}

void NotepadWidget::createNewFile()
{
    qDebug()<< "New file creating";
    fileName = "/home/kalach/" + userDialog->dialogLineEdit->text();
    saveFile();
}

void NotepadWidget::enableSaveButton() {
    saveButton->setEnabled(true);
    saveAsButton->setEnabled(true);
}

void NotepadWidget::disableSaveButton() {
    saveButton->setEnabled(false);
    saveAsButton->setEnabled(false);

}

UserDialog::UserDialog()
{
    dialogLabel = new QLabel;
    dialogLineEdit = new QLineEdit;
    okButton = new QPushButton;
    okButton->setText(tr("Ok"));
    cancelButton = new QPushButton;
    cancelButton->setText(tr("Cancel"));
    dialogMainLayout = new QVBoxLayout;
    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    dialogMainLayout->addWidget(dialogLabel);
    dialogMainLayout->addWidget(dialogLineEdit);
    dialogMainLayout->addLayout(buttonsLayout);
    this->setLayout(dialogMainLayout);
    this->setModal(true);
    connect(this->okButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(this->cancelButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}
