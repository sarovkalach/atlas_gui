#include "integratorwidget.h"

void IntegratorWidget::setAtlasIntegratorRelations(){
	integrateDataModel_->setRelation(5, QSqlRelation("atlas.integrator", "id", "name"));
	QString filter = QString("owner= %1").arg(owner_);
	integrateDataModel_->setFilter(filter);
}

void IntegratorWidget::showIntegratetableModel(int owner){
	if (owner != owner_){
		owner_ = owner;
		qDebug()<<"owner = "<<owner_;
		setAtlasIntegratorRelations();
	}

	if (! integrateDataModel_->select() )
		QMessageBox::warning(Q_NULLPTR, tr("Connection broken"), tr("Can\'t execute select() from Database. May be connection with Database is broken?"));
}

void IntegratorWidget::startIntegrate(){
	qDebug()<<"Start integrate...";

	qDebug()<<"catalog = "<<catalog_;

	puskach_.start(catalog_, allIntegrators_[integratorsCB_->currentIndex()]);
	showIntegratetableModel(owner_);
}

void IntegratorWidget::deleteRecords(){
	qDebug()<<"I can\'t delete records now! please, define me!";
}

void IntegratorWidget::showNotepad(){
	qDebug()<<"Show notepad";
	if (notepad_->isVisible()){
		notepad_->hide();
	}
	else{
		notepad_->show();
	}
}

void IntegratorWidget::showAnotherDescription(int ind){
	integratorDescription_->setText(allIntegrators_[ind].description);
	if (integratorDescription_->toPlainText().size() > 0)
		integratorDescription_->show();
	else
		integratorDescription_->hide();
}

void IntegratorWidget::addComment(){
	QString comm = puskach_.getComment();
	TextDialog::createTextDialog(comm,this);
	puskach_.setComment(comm);
}

void IntegratorWidget::chooseInFiles(){
	qDebug()<<"FROM IntegratorWidget::chooseCatalog() :";
	bool ok = false;
	QStringList inFiles;
	QString catalog;
	while(!ok){
		inFiles = QFileDialog::getOpenFileNames(this, tr("Choose in-files for integrator"), puskach_.init());

		if (inFiles.size() == 0){
			return;
		}
		else{
			typename QStringList::const_iterator it = inFiles.cbegin();
			catalog = it->left(it->lastIndexOf("/") + 1);
			qDebug()<<"catalog = "<<catalog;
			QString onlyFilename;
			for(; it != inFiles.cend(); ++it){
				onlyFilename = it->right(it->size() - it->lastIndexOf("/") - 1);
				qDebug()<<"fromlistFilename = "<<onlyFilename;
				if (it->contains(";")){
					QMessageBox::warning(this, tr("Wrong filename"), tr("Filenames can\'t contains ';' symbol. Please, select another files"));
					break;
				}
			}
			if (it == inFiles.cend())
				ok = true;
		}
	}

	catalog_ = catalog;
	qDebug()<<"catalog = "<<catalog;
	qDebug()<<"catalog_ = "<<catalog_;

	notepad_->set_defaultDir(catalog);

	typename QStringList::iterator it = inFiles.begin();
	string vs;
	for(; it != inFiles.end(); ++it){
		vs = it->toStdString();
		*it = QString::fromStdString(vs.substr( vs.find_last_of("/")+1 ));
	}

	puskach_.setInFiles(inFiles);

	notepadButton_->setEnabled(true);
	startButton_->setEnabled(true);
}




void IntegratorWidget::initAllIntegrators(){
	setIntegratorLabel_ = new QLabel(tr("Set converter:"));
	integratorsCB_ = new QComboBox;

	QSqlQuery query;
	QSqlQueryModel queryModel;

	query.prepare("SELECT id,name,reference,description FROM atlas.integrator");
	if (!query.exec()){
		QMessageBox::warning(Q_NULLPTR, tr("Warning"), tr("Can\'t execute query. May be connection with Database is broken?"));
		return;
	}

	queryModel.setQuery(query);
	int N = queryModel.rowCount();
	allIntegrators_.reserve(N);

	int id;
	QString name, ref, description;
	for(int i=0; i<N; ++i){
		id = queryModel.data(queryModel.index(i,0)).toInt();
		name = queryModel.data(queryModel.index(i,1)).toString();
		ref = queryModel.data(queryModel.index(i,2)).toString();;
		description = queryModel.data(queryModel.index(i,3)).toString();

		allIntegrators_.push_back(TableIntegrElem(id,name,ref,description));

		integratorsCB_->addItem(name);
	}

	if (N > 0){
		integratorsCB_->setCurrentIndex(0);
		showAnotherDescription(0);
	}

	connect(integratorsCB_, SIGNAL(currentIndexChanged(int)), this, SLOT(showAnotherDescription(int)));
}

void IntegratorWidget::initAllButtons(){
	buttonLayout_ = new QHBoxLayout;

	startButton_ = new QPushButton(tr("Start"));
	deleteButton_ = new QPushButton(tr("Delete"));
	commentButton_ = new QPushButton(tr("Add comment..."));
	chooseInFilesButton_ = new QPushButton(tr("Choose InFiles..."));
	notepadButton_ = new QPushButton(tr("Notepad"));

	startButton_->setEnabled(false);
	notepadButton_->setEnabled(false);

	connect(startButton_, SIGNAL(clicked()), this, SLOT(startIntegrate()));
	connect(deleteButton_, SIGNAL(clicked()), this, SLOT(deleteRecords()));
	connect(commentButton_, SIGNAL(clicked()), this, SLOT(addComment()));
	connect(chooseInFilesButton_, SIGNAL(clicked()), this, SLOT(chooseInFiles()));
	connect(notepadButton_, SIGNAL(clicked()), this, SLOT(showNotepad()));

	buttonLayout_->addWidget(startButton_);
	buttonLayout_->addWidget(deleteButton_);
	buttonLayout_->addWidget(commentButton_);
	buttonLayout_->addWidget(chooseInFilesButton_);
	buttonLayout_->addWidget(notepadButton_);
}

void IntegratorWidget::initMainTable(){
	integrateDataModel_ = new QSqlRelationalTableModel;
	integrateDataView_ = new QTableView;

	integrateDataModel_->setTable("atlas.integrate");
	integrateDataView_->setEditTriggers(QTableView::NoEditTriggers);
	integrateDataView_->setModel(integrateDataModel_);
	integrateDataView_->horizontalHeader()->setStretchLastSection(true);

	setAtlasIntegratorRelations();

	integrateDataModel_->setHeaderData(2, Qt::Horizontal, tr("Date"));
	integrateDataModel_->setHeaderData(3, Qt::Horizontal, tr("InFile"));
	integrateDataModel_->setHeaderData(4, Qt::Horizontal, tr("OutFile"));
	integrateDataModel_->setHeaderData(5, Qt::Horizontal, tr("Integrator Name"));
	integrateDataModel_->setHeaderData(6, Qt::Horizontal, tr("Comment"));
	integrateDataModel_->setHeaderData(7, Qt::Horizontal, tr("Catalog"));

	integrateDataView_->hideColumn(0);
	integrateDataView_->hideColumn(1);
}

void IntegratorWidget::init(){
	mL_ = new QVBoxLayout;
	notepad_ = new SimpleArtificialShell;
	integratorDescription_ = new QTextEdit;
	integratorDescription_->setFixedHeight(50);

	notepad_->hide();
	integratorDescription_->setReadOnly(true);

	initAllIntegrators();
	initAllButtons();
	initMainTable();

	mL_->addWidget(setIntegratorLabel_);
	mL_->addWidget(integratorsCB_);
	mL_->addWidget(integratorDescription_);
	mL_->addLayout(buttonLayout_);
	mL_->addWidget(notepad_);
	mL_->addWidget(integrateDataView_);

	setLayout(mL_);
}

IntegratorWidget::IntegratorWidget(int owner, QWidget *pwg)
: QWidget(pwg), owner_(owner), puskach_(owner_){
	init();
}
