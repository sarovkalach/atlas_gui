#include "../general_gui_files/include/textdialog.h"


// ----------------------------- TextDialog

TextDialog::TextDialog(QString &text, QWidget *pwg) : QDialog(pwg), text_(text){
	mL_ = new QVBoxLayout;
	editor_ = new QTextEdit;
	okButton_ = new QPushButton(tr("OK"));

	editor_->setText(text_);

	mL_->addWidget(editor_);
	mL_->addWidget(okButton_);
	setLayout(mL_);

	connect(okButton_, SIGNAL(clicked()), this, SLOT(pushOk()));
}

void TextDialog::pushOk(){
	text_ = editor_->toPlainText();
	accept();
}
