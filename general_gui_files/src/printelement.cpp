#include "../general_gui_files/include/printelement.h"

//---------- PrintElement

size_t fttw::PrintElement::nextSerialNum = 0;

//---------- TablePrintElem

ostream& fttw::TablePrintElem::printElem(ostream &s) const{
	if (name_ != nullptr){
		s<<"#dataname"<<(name_).toStdString()<<endl;
	}

	s<<"#data";
	size_t Nrow, Ncol;
	if (table_ == nullptr){
		cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\ttable was not declared in TablePrintElem"<<endl;
		exit(-1);
	}

	Nrow = table_->rowCount();
	Ncol = table_->columnCount();

	for(size_t j=0; j<Ncol; ++j){
		s<<" "<<table_->horizontalHeaderItem(j)->text().toStdString();
	}
	s<<endl;

	for(size_t j=0; j<Nrow; ++j){
		for(size_t k=0; k<Ncol-1; ++k){
			s<<table_->item(j,k)->text().toStdString()<<"\t";
		}
		s<<table_->item(j,Ncol-1)->text().toStdString()<<endl;
	}
//	s<<endl;
	return s;
}

//---------- GlobalValuePrintElem

ostream& fttw::GlobalValuePrintElem::printElem(ostream &s) const{
	if (label_ == nullptr){
		cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tLabel was not declared in GlobalValuePrintElem"<<endl;
		exit(-1);
	}
	if (editor_ == nullptr){
		cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tLineEditor was not declared in GlobalValuePrintElem"<<endl;
		cout<<"Label = "<<label_->text().toStdString()<<endl;
		exit(-1);
	}

	s<<"#head "<<(label_->text()).toStdString()<<endl;
	s<<(editor_->text()).toStdString()<<endl;
	return s;
}

//---------- GlobalValuesPrintElem

ostream& fttw::GlobalValuesPrintElem::printElem(ostream &s) const{
	if (labels_.size() == 0){
		cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tLabels were not declared in GlobalValuesPrintElem"<<endl;
		exit(-1);
	}
	if (labels_.size() != editors_.size()){
		cout<<"ERROR in "<<__FUNCTION__<<"(ostream&) const : line "<<__LINE__<<", file "<<__FILE__<<endl;
		cout<<"\tNumber of labels are not equal to the number of line editors in GlobalValuesPrintElem"<<endl;
		exit(-1);
	}

	s<<"#head";
	size_t N = labels_.size();
	for(size_t i=0; i<N; ++i){
		s<<' '<<labels_[i]->text().toStdString();
	}
	s<<endl;

	s<<editors_[0]->text().toStdString();
	for(size_t i=1; i<N; ++i){
		s<<' '<<editors_[i]->text().toStdString();
	}
	s<<endl;
	return s;
}

//---------- CommentElement

ostream& fttw::CommentElement::printElem(ostream &s) const{
	s<<str_<<endl;
	return s;
}
