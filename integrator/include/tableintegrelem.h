#ifndef TABLEINTEGRELEM_GASPARYANMOSES12042017
#define TABLEINTEGRELEM_GASPARYANMOSES12042017

#include <QString>

/*! \brief Элемент из таблицы atlas.integrator .
 *
 * Структура, которая описывает полностью элемент из
 * таблицы в базе данных atlas.integrator .
 *
 * Т.к. данная таблица является статической, т.е. в
 * процессе работы программ не может быть изменена, то
 * можно считать один раз в программе все поля, а затем
 * с ними работать. Для этого и нужна данная структура.
 *
 * Схема использования:
 * \code
 *
 *	QSqlQuery query;
 *	QSqlQueryModel queryModel;
 *
 *	query.prepare("SELECT id,name,reference,description FROM atlas.integrator");
 *	query.exec();
 *
 *	queryModel.setQuery(query);
 *	int N = queryModel.rowCount();
 *
 *	vector<TableIntegrElem> allIntegrators;
 *	allIntegrators.reserve(N);
 *
 *	int id;
 *	QString name, ref, description;
 *	for(int i=0; i<N; ++i){
 *		id = queryModel.data(queryModel.index(i,0)).toInt();
 *		name = queryModel.data(queryModel.index(i,1)).toString();
 *		ref = queryModel.data(queryModel.index(i,2)).toString();;
 *		description = queryModel.data(queryModel.index(i,3)).toString();
 *
 *		allIntegrators.push_back(TableIntegrElem(id,name,ref,description));
 *	}
 *
 * \endcode
 *
 */
struct TableIntegrElem{
	//! Идентификационный номер конвертера.
	int id {0};
	//! Имя конвертера, которое будет показано пользователю в выпадающем списке.
	QString name {""};
	//! Расположение исполняемого файла для данного конвертера.
	QString ref {""};
	//! Описание конвертера. Описание может отсутствовать.
	QString description {""};

	//! Пустой конструктор.
	TableIntegrElem() {}
	//! Конструктор со всеми заданными полями.
	TableIntegrElem(const int& id_in, const QString& name_in, const QString& ref_in,const QString& description_in) : id(id_in), name(name_in), ref(ref_in), description(description_in) {}
};

#endif // TABLEINTEGRELEM_GASPARYANMOSES12042017
