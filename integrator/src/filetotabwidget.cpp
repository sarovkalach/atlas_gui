#include "filetotabwidget.h"

//------------------ GLOBAL FUNCTIONS

template<typename T>
string fttw::toStr(T elem){
	stringstream vss;
	vss<<elem;
	return vss.str();
}

void fttw::print_mistake(const string &funcName, int line, const string &fileName, const string &message){
	cerr<<"ERROR in "<<funcName<<" : line "<<line<<", file "<<fileName<<endl;
	cerr<<"\t"<<message<<endl;
	exit(-1);
}
void fttw::print_mistakeLite(const string &funcName, int line, const string &fileName, const string &message){
	cerr<<"WARNING in "<<funcName<<" : line "<<line<<", file "<<fileName<<endl;
	cerr<<"\t"<<message<<endl;
}

string fttw::getOneLine(istream &ss){
	string res = "";
	char c = 0;
	ss>>noskipws;
	ss>>c;
	while((c != '\n') && (!ss.eof())){
		res.push_back(c);
		ss>>c;
	}
	ss>>skipws;
	return res;
}

bool fttw::getOneLine(istream &ss, string &vs){
	ss>>noskipws;
	vs = "";
	char c = 0;

	ss>>c;
	if ((c != '\n') && (!ss.eof()))
		vs.push_back(c);
	else{
		ss>>skipws;
		return true;
	}
	while(ss>>c){
		if (c != '\n')
			vs.push_back(c);
		else{
			ss>>skipws;
			return true;
		}
	}

	ss>>skipws;
	return false;
}

bool fttw::isDouble(const string &s){
	QString vs = QString::fromStdString(s);
	bool res = false;
	vs.toDouble(&res);
	return res;
}

template<typename T>
T fttw::fromStringTo(const string& s){
	stringstream vss;
	vss.str(s);
	T res;
	vss>>res;
	return res;
}

ostream& fttw::print_parent(ostream &s, const QObject *obj){
	if (obj->parent() != nullptr)
		s<<"parent = "<<obj->parent()->objectName().toStdString();
	else
		s<<"parent = nullptr";
	return s;
}

ostream& fttw::tryToPrint(ostream &s, const QObject *obj){
	if (obj != nullptr){
		s<<obj->objectName().toStdString()<<", ";
		print_parent(s, obj);
	}
	else
		s<<"nullptr";
	return s;
}


//------------------ OnlyDoubleDelegate

QWidget* fttw::OnlyDoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
	QLineEdit *lineEdit = new QLineEdit(parent);
	QDoubleValidator *validator = new QDoubleValidator(-numeric_limits<double>::max(), numeric_limits<double>::max(), 15, lineEdit);
	validator->setLocale(QLocale(QLocale::English));
	lineEdit->setValidator(validator);
	return lineEdit;
}


//------------------ HighLightNumbers

void fttw::HighLightNumbers::init(){
	intFormat_.setForeground(Qt::darkRed);
	doubleFormat_.setForeground(Qt::red);
	keywordFormat_.setForeground(Qt::blue);

	HighlightRule rule;

	QStringList keyWords;
	keyWords<<"^#data\\b"
			<<"^#dataname\\b"
			<<"^#head\\b";
	for(QStringList::iterator it = keyWords.begin(); it != keyWords.end(); ++it){
		rule.format = keywordFormat_;
		rule.pattern = QRegExp(*it);
		rules_.push_back(rule);
	}

	rule.format = intFormat_;
	rule.pattern = QRegExp("[\\+\\-]?\\b[0-9][0-9]*\\b");
	rules_.push_back(rule);

	QStringList doublePatterns;
	doublePatterns<<"[\\+\\-]?\\b[0-9]*\\.[0-9]*\\b"
				  <<"[\\+\\-]?\\b[0-9]*\\.?[0-9][eE][\\-\\+]?[0-9][0-9]*\\b";
	for(QStringList::iterator it = doublePatterns.begin(); it != doublePatterns.end(); ++it){
		rule.format = doubleFormat_;
		rule.pattern = QRegExp(*it);
		rules_.push_back(rule);
	}
}

void fttw::HighLightNumbers::highlightBlock(const QString &str){
	int index;
	int L;
	for(int i = 0; i < rules_.size(); ++i){
		QRegExp expression(rules_[i].pattern);
		index = expression.indexIn(str);
		while(index >= 0){
			L = expression.matchedLength();
			setFormat(index, L, rules_[i].format);
			index = expression.indexIn(str, index + L);
		}
	}
}


//------------------ FileToTabWidget

ostream& fttw::FileToTabWidget::print(ostream& s) const{
	s<<"FileToTabWidget:"<<endl;
	s<<"  name = "<<objectName().toStdString()<<endl;
	s<<"  ";
	print_parent(s, this);
	s<<endl<<endl;

	s<<"  mainWidget = "<<mW_->objectName().toStdString()<<endl;
	s<<"  parent of mainWidget = ";
	print_parent(s, mW_);
	s<<endl;
	s<<"  mainLayout = ";
	tryToPrint(s, mL_);
	s<<endl;

	s<<"  unmarkedValues_ = ";
	tryToPrint(s, unmarkedValues_);
	s<<endl<<endl;

	s<<"  tabsWidget = ";
	tryToPrint(s, tabs_);
	s<<endl<<endl;

	s<<"  all globalValues (N = "<<globalValues_.size()<<"):"<<endl;
	for(size_t i=0; i<globalValues_.size(); ++i){
		s<<"    glV["<<i<<"].first = ";
		tryToPrint(s, globalValues_[i].first);
		s<<endl;
		s<<"    glV["<<i<<"].second = ";
		tryToPrint(s, globalValues_[i].second);
		s<<endl;
	}
	s<<endl;

	s<<"  all tables (N = "<<tables_.size()<<"):"<<endl;
	for(size_t i=0; i<tables_.size(); ++i){
		s<<"    tb["<<i<<"] = ";
		tryToPrint(s, tables_[i]);
		s<<endl;
	}
	s<<endl;
	return s;
}

ostream& fttw::operator<<(ostream& s, const FileToTabWidget& tw){
	return tw.print(s);
}

void fttw::FileToTabWidget::layout_init(){
	mL_ = new QVBoxLayout;
	mW_->setLayout(mL_);
	mL_->setObjectName("mainLayout");
}

inline void fttw::FileToTabWidget::main_init(){
	setObjectName("ScrollArea");
	mW_ = new QFrame;
	mW_->setObjectName("mainWidget");
	layout_init();
	setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	setWidget(mW_);
	setWidgetResizable(true);
}

void fttw::FileToTabWidget::createFileBuffer(const string &filename, stringstream &s) const{
	ifstream fin;
	fin.open(filename.c_str(), ios::in);
	if (!fin.is_open()){
		fttw::print_mistake(toStr(__FUNCTION__)+"(const string&, stringstream&)", __LINE__, __FILE__, "can\'t open file with name\n"+toStr(filename));
	}

	string vs = "";
	getline(fin,vs);
	while(!fin.eof()){
		s<<vs<<'\n';
		getline(fin, vs);
	}
	s<<vs<<'\n';

	fin.close();
}

void fttw::FileToTabWidget::createTextEditor(const string &filename){
	cout<<"CREATING TEXT EDITOR..."<<endl;

	unmarkedValues_ = new QTextEdit(mW_);
	unmarkedValues_->setObjectName("text_editor");

	stringstream vss;
	createFileBuffer(filename, vss);

	unmarkedValues_->setText(QString::fromStdString(vss.str()));
	unmarkedValues_->setTabStopWidth(30);

	textEditorHighLighter_ = new HighLightNumbers(unmarkedValues_);
	textEditorHighLighter_->setObjectName("textEditor_highlighter");

	mL_->addWidget(unmarkedValues_);

	cout<<"TEXT EDITOR WAS CREATED"<<endl;
}

void fttw::FileToTabWidget::createLabelVector(const string &labels, vector<string> &vec) const{
	stringstream vss;		//поток для чтения названий глобальных параметров в этой строчке
	vss.str(labels);
	vec.reserve(5);
	string vs;
	while(vss>>vs){			//считываем все названия и производим подсчет количества параметров в данной строке
		vec.push_back(vs);
	}
}

size_t fttw::FileToTabWidget::readGlobalValues(istream& ss, const string &labelLine){
	vector<string> labels;		//названия глобальных параметров
	createLabelVector(labelLine, labels);
	vector<string> values;		//считанные значения глобальных параметров
	values.reserve(labels.size());

	size_t N = labels.size();	//кол-во глобальных параметров, которое требуется
							//считываем значения параметров
	string vs;
	for(size_t i=0; i<N; ++i){
		vs = "";
		ss>>vs;
		values.push_back(vs);
	}
							//создаем метку и текстовое поле для каждого из параметров
	QLabel *oneLabel;
	QLineEdit *oneLineEdit;
	QHBoxLayout *oneLayout;
	QDoubleValidator *validator;
	QString valName;
	globalValues_.reserve(N);
	for(size_t i=0; i<N; ++i){
		valName = QString::fromStdString(labels[i]);
		oneLayout = new QHBoxLayout;
		oneLayout->setObjectName("layoutFor_"+valName);
		oneLabel = new QLabel(valName, mW_);
		oneLabel->setObjectName("labelFor_"+valName);
		oneLineEdit = new QLineEdit(QString::fromStdString(values[i]), mW_);
		oneLineEdit->setObjectName("lineEditFor_"+valName);

		validator = new QDoubleValidator(-numeric_limits<double>::max(), numeric_limits<double>::max(), 15, oneLineEdit);
		validator->setObjectName("validatorFor_"+valName);
		validator->setLocale(QLocale::English);
		oneLineEdit->setValidator(validator);

		oneLayout->addWidget(oneLabel);
		oneLayout->addWidget(oneLineEdit);
		mL_->addLayout(oneLayout);
		globalValues_.push_back(pair<QLabel*, QLineEdit*>(oneLabel, oneLineEdit));
	}
	return N;
}

void fttw::FileToTabWidget::addTableToTab(QTableWidget *table){
	if (tabs_ == nullptr){
		tabs_ = new QTabWidget(mW_);
		tabs_->setObjectName("tabsArray");
	}
	tabs_->addTab(table, QString::number(tables_.size()));
	table->setObjectName("table_"+QString::number(tables_.size()));
}

void fttw::FileToTabWidget::addTableToTab(QTableWidget *table, const QString &tabname){
	if (tabs_ == nullptr){
		tabs_ = new QTabWidget(mW_);
		tabs_->setObjectName("tabsArray");
	}
	tabs_->addTab(table, tabname);
	table->setObjectName("table_"+tabname);
}

size_t fttw::FileToTabWidget::readTable(istream &ss, const string &labels){
	vector<string> vecLabels;				//названия столбцов
	createLabelVector(labels, vecLabels);
	size_t Ncol = vecLabels.size();			//количество столбцов

	QStringList listLabels;					//создаем QStringList из вектора
	for(size_t i=0; i<Ncol; ++i){
		listLabels<<QString::fromStdString(vecLabels[i]);
	}

	vector<vector<string> > data;			//данные из файла, которые будут изначально в таблице
	string vs;
	vs.reserve(10);
	size_t Nrow = 0;

	size_t numberOfEntersAfterTable = 0;
	char cheker = 0;
	while(ss>>vs){							//считывание данных
		for(size_t i=0; i<vs.size(); ++i){
			ss.unget();
		}
		if (isDouble(vs)){
			Nrow = data.size();
			data.resize(Nrow+1);
			++Nrow;
			data[Nrow-1].resize(Ncol);
			for(size_t i=0; i<Ncol; ++i){
				vs = "";
				ss>>vs;
				data[Nrow-1][i] = vs;
			}
		}
		else {
			break;
		}
		vs = "";

		ss>>noskipws;
		while(ss.peek() == '\n'){
			ss>>cheker;
			++numberOfEntersAfterTable;
		}
		if (numberOfEntersAfterTable == 1) {numberOfEntersAfterTable = 0;}
		ss>>skipws;
	}
	if (numberOfEntersAfterTable != 0) {--numberOfEntersAfterTable;}
	Nrow = data.size();

	QTableWidget* table = new QTableWidget(Nrow, Ncol, mW_);
	table->setItemDelegate(new OnlyDoubleDelegate);
	table->setHorizontalHeaderLabels(listLabels);

	listLabels.clear();
	for(size_t i=0; i<Nrow; ++i){
		listLabels<<QString::number(i+1);
	}
	table->setVerticalHeaderLabels(listLabels);

	QTableWidgetItem* item;
	for(size_t i=0; i<Nrow; ++i){
		for(size_t k=0; k<Ncol; ++k){
			item = new QTableWidgetItem(QString::fromStdString(data[i][k]));
			table->setItem(i,k, item);
		}
	}

	tables_.push_back(table);
	return numberOfEntersAfterTable;
}

QString fttw::FileToTabWidget::getTabName(istream &ss) const{
	string vs;
	getOneLine(ss, vs);
	QString res;
	res = QString::fromStdString(vs);

	if (vs.size() == 0){
		print_mistakeLite(toStr(__FUNCTION__)+"(istream&)", __LINE__, __FILE__, "tab name is empty");
	}

	return res;
}

void fttw::FileToTabWidget::setWidthFromTables(){
	int w, maxw = 0;
	size_t maxind = 0;
	size_t N = tables_.size();

	for(size_t i=0; i<N; ++i){
		w = tables_[i]->size().width()*tables_[i]->columnCount();
		if (maxw < w){
			maxw = w;
			maxind = i;
		}
	}

	if (N > 0){
		maxw = tables_[maxind]->size().width()*(tables_[maxind]->columnCount() + 1);
		resize(maxw, size().height());
	}
}

bool fttw::FileToTabWidget::checkGlobalValues(istream &ss, const string &labels) const{
	stringstream vss;		//поток для чтения названий глобальных параметров в этой строчке
	vss.str(labels);
	string vs;
	size_t N = 0;
	while(vss>>vs){			//считываем все названия и производим подсчет количества параметров в данной строке
		++N;
	}

	for(size_t i=0; i<N; ++i){
		vs = "";
		ss>>vs;
		if ((vs.size() == 0) || !isDouble(vs)){
			print_mistakeLite(toStr(__FUNCTION__)+"(stringstream&, const string&)", __LINE__, __FILE__, "too few values for defining all global parameters. vs=\""+toStr(vs)+"\" is not a double.");
			return false;
		}
	}
	return true;
}

bool fttw::FileToTabWidget::checkTable(istream &ss, const string &labels) const{
	stringstream vss;		//поток для чтения названий глобальных параметров в этой строчке
	vss.str(labels);
	string vs;

	size_t Ncol = 0;
	while(vss>>vs)			//считываем все названия и производим подсчет количества параметров в данной строке
		++Ncol;
	if (Ncol == 0)
		return false;

	vs = "";
	while(ss>>vs){							//считывание данных
		for(size_t i=0; i<vs.size(); ++i)
			ss.unget();
		if (isDouble(vs)){
			//++Nrow;
			for(size_t i=0; i<Ncol; ++i){
				vs = "";
				ss>>vs;
				if ((vs.size() == 0) || !isDouble(vs))
					return false;
			}
		}
		else
			break;
		vs = "";
	}

	return true;
}

bool fttw::FileToTabWidget::check_correct(const string &filename) const{
	cout<<"CHECKING FILE FOR CORRECT INPUT..."<<endl;

	bool res = false;
	stringstream fin;
	createFileBuffer(filename, fin);

	string vsLine;
	QString vs;

	fin>>vsLine;
	while(!fin.eof()){
		if (!vsLine.compare(0, 5, "#head", 0, 5)){
			getOneLine(fin, vsLine);
			res = checkGlobalValues(fin, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "global values writed incorrectly");
				break;
			}
		}
		else if(!vsLine.compare(0, 9, "#dataname", 0, 9)){		//таблица вместе с именем вкладки
			vs = getTabName(fin);
			fin>>vsLine;

			getOneLine(fin, vsLine);
			res = checkTable(fin, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "table number "+toStr(tables_.size()+1)+" writed incorrectly");
				break;
			}
		}
		else if(!vsLine.compare(0, 5, "#data", 0, 5)){		//таблица
			getOneLine(fin, vsLine);
			res = checkTable(fin, vsLine);
			if (!res){
				print_mistakeLite(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "table number "+toStr(tables_.size()+1)+" writed incorrectly");
				break;
			}
		}
		else if(vsLine[0] == '#'){							//комментарий
			getOneLine(fin, vsLine);
		}
		else if (vsLine.size() > 0){						//неразмеченная строка
			res = false;
			break;
		}
		fin>>vsLine;
	}

	cout<<"CHECKING IS COMPLITE"<<endl;
	return res;
}

void fttw::FileToTabWidget::createTabsAndGlobalValues(const string &filename){
	cout<<"START LOADING FILE..."<<endl;

	stringstream fin;
	createFileBuffer(filename, fin);

	string vsLine;
	QString vs;
	fin>>vsLine;
	char c='a';

	size_t numberOfEnters = 0;
	bool flagComment = false;
	size_t numberOfGlobalValuesInOneString = 0;

	while(!fin.eof()){
		if (!vsLine.compare(0, 5, "#head", 0, 5)){			//глобальные параметры
			getOneLine(fin, vsLine);
			numberOfGlobalValuesInOneString = readGlobalValues(fin, vsLine);
			if (numberOfGlobalValuesInOneString == 1)
				printElements_.push_back(new GlobalValuePrintElem(globalValues_[globalValues_.size()-1].first, globalValues_[globalValues_.size()-1].second));
			else{
				GlobalValuesPrintElem* gvpe = new GlobalValuesPrintElem;
				for(size_t i = globalValues_.size() - numberOfGlobalValuesInOneString; i<globalValues_.size(); ++i){
					gvpe->addLabel(globalValues_[i].first);
					gvpe->addEditor(globalValues_[i].second);
				}

				printElements_.push_back(gvpe);
			}
			//sdfadolfja;dsfija;sldfkj;asiufas dddddddddddddddddddddddddddddddddddddddddddddddddddddd!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
		else if(!vsLine.compare(0, 9, "#dataname", 0, 9)){		//таблица вместе с именем вкладки
			vs = getTabName(fin);
			TablePrintElem* tpe = new TablePrintElem;
			tpe->setName(vs);

			fin>>vsLine;

			getOneLine(fin, vsLine);
			numberOfEnters = readTable(fin, vsLine);
			addTableToTab(tables_[tables_.size()-1], vs);
			tpe->setTable(tables_[tables_.size()-1]);

			printElements_.push_back(tpe);
		}
		else if(!vsLine.compare(0, 5, "#data", 0, 5)){		//таблица
			getOneLine(fin, vsLine);
			numberOfEnters = readTable(fin, vsLine);
			addTableToTab(tables_[tables_.size()-1]);

			printElements_.push_back(new TablePrintElem(tables_[tables_.size()-1]));
		}
		else if(vsLine[0] == '#'){							//комментарий
			string oldvs = vsLine;
			getOneLine(fin, vsLine);
			oldvs += vsLine;

			printElements_.push_back(new CommentElement(oldvs));
			flagComment = true;
		}
		else if (vsLine.size() > 0){						//неразмеченная строка
			print_mistake(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "check function for checking - they miss something, because file have unmarked string");
		}

		if (numberOfEnters > 0){
			for(size_t i=0; i<numberOfEnters; ++i){
				printElements_.push_back(new CommentElement);
			}
			numberOfEnters = 0;
		}

		fin>>noskipws;
		while(fin.peek() == '\n'){
			fin>>c;
			++numberOfEnters;
		}
		fin>>skipws;

		if (numberOfEnters > 0){
			if (!flagComment) {--numberOfEnters;}
			for(size_t i=0; i<numberOfEnters; ++i){
				printElements_.push_back(new CommentElement);
			}
			numberOfEnters = 0;
		}

		vsLine = "";
		fin>>vsLine;
		flagComment = false;
	}

	if (tabs_ != nullptr){
		mL_->addWidget(tabs_);
	}

	setWidthFromTables();
	cout<<"LOADING FILE HAS COMPLETED"<<endl;
}

inline void fttw::FileToTabWidget::load_file(const string &filename){
	if ((mode_ == Tabs) && check_correct(filename)){
		createTabsAndGlobalValues(filename);
	}
	else{
		createTextEditor(filename);
		mode_ = TextEditor;
	}
}

fttw::FileToTabWidget::FileToTabWidget(const string &filename, QWidget *pwg, mode in_mode) : QScrollArea(pwg), mode_(in_mode){
	main_init();
	load_file(filename);
}

void fttw::FileToTabWidget::saveFromData(ostream& fout) const{
	for(size_t i=0; i<printElements_.size(); ++i){
		printElements_[i]->printElem(fout);
	}
}

void fttw::FileToTabWidget::saveFromEditor(ostream& fout) const{
	fout<<unmarkedValues_->toPlainText().toStdString();
}

void fttw::FileToTabWidget::save_file(const string &filename) const{
	ofstream fout;
	fout.open(filename.c_str(), ios::out);
	if (!fout.is_open()){
		print_mistake(toStr(__FUNCTION__)+"(const string&)", __LINE__, __FILE__, "can\'t create or edit file with name\n"+filename);
	}

	if (unmarkedValues_ == nullptr){
		cout<<"SAVING FROM DATA..."<<endl;
		saveFromData(fout);
	}
	else{
		cout<<"SAVING FROM EDITOR..."<<endl;
		saveFromEditor(fout);
	}
	cout<<"SAVING COMPLITE"<<endl;

	fout.close();
}


//----------------------- OutSideWidget

void fttw::OutSideWidget::savedata_slot(){
	emit savedata(savename_);
}

void fttw::OutSideWidget::reload_slot(){
	mL_->removeWidget(fttw_);
	disconnect(this, SIGNAL(savedata(const string&)), fttw_, SLOT(save_file(const string&)));
	delete fttw_;

	fttw_= new FileToTabWidget(loadname_, this);
	mL_->insertWidget(0, fttw_);

	connect(this, SIGNAL(savedata(const string&)), fttw_, SLOT(save_file(const string&)));
	if (fttw_->getMode() == FileToTabWidget::Tabs){
		toTextEditorButton_->show();
	}
	else{
		toTextEditorButton_->hide();
	}
}

void fttw::OutSideWidget::toTextEditor(){
	emit savedata(savename_);

	mL_->removeWidget(fttw_);
	delete fttw_;

	fttw_ = new FileToTabWidget(loadname_, this, FileToTabWidget::TextEditor);
	mL_->insertWidget(0, fttw_);

	connect(this, SIGNAL(savedata(const string&)), fttw_, SLOT(save_file(const string&)));
	if (fttw_->getMode() == FileToTabWidget::Tabs){
		toTextEditorButton_->show();
	}
	else{
		toTextEditorButton_->hide();
	}
}

void fttw::OutSideWidget::init(){
	fttw_= new FileToTabWidget(loadname_, this);
	mL_ = new QVBoxLayout;
	setLayout(mL_);
	setFrameStyle(NoFrame);

	saveButton_ = new QPushButton("&Save");
	reloadButton_ = new QPushButton("&Reload");
	toTextEditorButton_ = new QPushButton("&Text Editor");

	mL_->addWidget(fttw_);
	mL_->addWidget(toTextEditorButton_);
	QHBoxLayout *saveReloadLayout = new QHBoxLayout;
	saveReloadLayout->addWidget(saveButton_);
	saveReloadLayout->addWidget(reloadButton_);
	mL_->addLayout(saveReloadLayout);
//	mL_->addWidget(saveButton_);
//	mL_->addWidget(reloadButton_);

	connect(toTextEditorButton_, SIGNAL(clicked()), this, SLOT(toTextEditor()));
	if (fttw_->getMode() == FileToTabWidget::TextEditor)
		toTextEditorButton_->hide();
	connect(saveButton_, SIGNAL(clicked()), this, SLOT(savedata_slot()));
	connect(reloadButton_, SIGNAL(clicked()), this, SLOT(reload_slot()));
	connect(this, SIGNAL(savedata(const string&)), fttw_, SLOT(save_file(const string&)));
}

fttw::OutSideWidget::OutSideWidget(const string &name, QWidget *pwg)
: QFrame(pwg), savename_(name), loadname_(name){
	init();
}

fttw::OutSideWidget::OutSideWidget(const string& savename, const string& loadname, QWidget* pwg)
: QFrame(pwg), savename_(savename), loadname_(loadname){
	init();
}
