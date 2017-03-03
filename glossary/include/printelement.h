/** @addtogroup oscillators
 * @{*/

/** @file */

#ifndef PRINTELEMENT13022017GASPARYANMOSES_H
#define PRINTELEMENT13022017GASPARYANMOSES_H

#include <iostream>

#include<QTableWidget>
#include<QString>
#include<QLabel>
#include<QLineEdit>
using namespace std;

namespace fttw {


/*! \brief Класс для печати элемента в стиле оформления FileToTabWidget
 *
 * Является классом, от которого наследуются другие классы.
 * Объект данного класса может быть создан только внутри объектов,
 * классы которых отнаследованы от данного.
 *
 * Также класс записывает id каждого созданного элемента (начиная с 0).
 *
 * Имеет единственный виртуальный метод для вывода элемента
 * в стиле разметки из класса FileToTabWidget.
 *
 * Схема использования:
 * \code
 *
 * fttw::printElem *pe = new fttw::CommentElement("#comment");
 *
 * //распечатаем содержимое комментария в поток вывода
 * pe->printElem(std::cout);
 * //выведет "#comment" + '\n'
 *
 * delete pe;
 *
 * \endcode
 *
 */
class PrintElement{
	//! Статическая переменная для выдачи разных id для элементов данного класса. Выдает начиная с 0.
	static size_t nextSerialNum;
	//! Идентификационный номер (id) данного элемента.
	const size_t serialNum_;

	//! Запрет копирования элемента данного класса через конструктор копирования.
	PrintElement(const PrintElement&);
	//! Запрет копирования элемента данного класса через оператор присваивания.
	const PrintElement& operator=(const PrintElement&);

protected:
	//! Конструктор. Устанавливает идентификационный номер элемента. \details Изменяет статическую переменную nextSerialNum, защищенное поле serialNum_.
	PrintElement() : serialNum_(nextSerialNum++) {}

public:
	//! Деструктор.
	virtual ~PrintElement() {}

	//! Функция читает приватное поле идентификатора элемента. \details Читает приватное поле serialNum_.
	const size_t& getSerialNum() const {return serialNum_;}
	//! Абстрактная функция для печати данного элемента в стиле разметки из fttw::FileToTabWidget.
	virtual ostream& printElem(ostream& s) const =0;
};


/*! \brief Класс для печати Таблицы в стиле оформления FileToTabWidget
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Позволяет печатать таблицу в стиле оформления файла, который подается
 * в fttw::FileToTabWidget.
 *
 * Хранит в себе имя таблицы и указатель на таблицу.
 * Выдает таблицу в стиле:
 * 1. Если нет имени:
 *
 * #data Va Vb Vc
 *
 * 1 2 3
 *
 * 4 5 6
 *
 * 7 8 9
 *
 *
 * 2. Если есть имя:
 *
 * #dataname TableName
 *
 * #data Va Vb Vc
 *
 * 1 2 3
 *
 * 4 5 6
 *
 * 7 8 9
 *
 *
 * Схема использования:
 * \code
 *
 * QTableWidget *table = new QTableWidget;
 * //настройка таблицы, создание элементов таблицы
 * //...
 *
 * fttw::TablePrintElem *tpe = new fttw::TablePrintElem;
 * tpe -> setTable(table);
 *
 * //если есть имя:
 * tpe -> setName("TableName");
 *
 * //печать, например, в стандартный поток вывода
 * tpe -> printElem();
 *
 * \endcode
 *
 */
class TablePrintElem : public PrintElement{
	//! Указатель на таблицу, элементы и названия столбцов которой будут печататься.
	QTableWidget* table_;
	//! Название таблицы. Если его нет, то поле равно "".
	QString name_;

public:
	//! Пустой конструктор. Все поля должны быть тогда заданы методами set.
	TablePrintElem() : PrintElement(), table_(nullptr), name_("") {}
	//! Конструктор с указателем на таблицу.
	explicit TablePrintElem(QTableWidget* table) : PrintElement(), table_(table), name_("") {}
	//! Конструктор с указателем на таблицу и именем.
	TablePrintElem(QTableWidget* table, const QString& name) : PrintElement(), table_(table), name_(name) {}
	//! Деструктор.
	~TablePrintElem() {}

	//! Функция задает указатель на таблицу. \details Изменяет защищенное поле table_.
	void setTable(QTableWidget* table) {table_ = table;}
	//! Функция возвращает указатель на таблицу. \details Читает защищенное поле table_. Таблицу можно менять.
	QTableWidget* getTable() {return table_;}
	//! Функция возвращает указатель на таблицу. \details Читает защищенное поле table_. Таблицу менять нельзя.
	const QTableWidget* getTable() const {return table_;}
	//! Перегруженный оператор "стрелочка". Возвращает указатель на таблицу. \details Читает защищенное поле table_. Таблицу можно менять.
	QTableWidget* operator->() {return table_;}
	//! Перегруженный оператор "стрелочка". Возвращает указатель на таблицу. \details Читает защищенное поле table_. Таблицу нельзя менять.
	const QTableWidget* operator->() const {return table_;}
	//! Функция задает имя таблице. \details Изменяет защищенное поле name_.
	void setName(const QString& str) {name_ = str;}
	//! Функция возвращает имя таблицы. \details Читает защищенное поле name_. Имя можно менять.
	QString& getName() {return name_;}
	//! Функция возвращает имя таблицы. \details Читает защищенное поле name_. Имя менять нельзя.
	const QString& getName() const {return name_;}

	//! Печатает таблицу в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::TablePrintElem).
	ostream& printElem(ostream &s) const;
};


/*! \brief Класс для печати глобальной переменной в стиле оформления FileToTabWidget
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Позволяет печатать название и значение глобальной переменной в стиле
 * файла, который подается в fttw::FileToTabWidget.
 *
 * Хранит в себе указатель на метку с названием глобальной переменной,
 * текстовое поле со значением переменной.
 *
 * Выводит глобальную переменную в виде:
 *
 * #head Name
 *
 * Value
 *
 *
 * Т.е.:
 *
 * #head T
 *
 * 1000
 *
 *
 * Схема использования:
 * \code
 *
 * //создаем метку и поле для редактирования со значением
 * QLabel *label = new QLabel("T");
 * QLineEdit *le = new QLineEdit("1000");
 *
 * //создаем элемент данного класса
 * fttw::GlobalValuePrintElem glv = new fttw::GlobalValuePrintElem;
 *
 * glv -> setLabel(label);
 * glv -> setLineEditor(le);
 *
 * //вывод, например, в стандартный поток вывода
 * glv -> printElem(std::cout);
 *
 * delete glv;
 *
 * \endcode
 *
 */
class GlobalValuePrintElem : public PrintElement{
	//! Метка с названием глобальной переменной.
	QLabel* label_;
	//! Текстовый редактор для значения переменной.
	QLineEdit* editor_;

public:
	//! Пустой конструктор. Задает поля nullptr-ами. Все поля должны быть заданы позже функциями set.
	GlobalValuePrintElem() : PrintElement(), label_(nullptr), editor_(nullptr) {}
	//! Конструктор сразу с параметрами. Не требуется после него делать функции set.
	GlobalValuePrintElem(QLabel* label, QLineEdit* lineedit) : PrintElement(), label_(label), editor_(lineedit) {}
	//! Деструктор.
	~GlobalValuePrintElem() {}

	//! Функция задает метку с названием переменной. \details Изменяет защищенное поле label_.
	void setLabel(QLabel* label) {label_ = label;}
	//! Функция возвращает метку с названием переменной. \details Читает защищенное поле label_. Метка может быть изменена.
	QLabel* getLabel() {return label_;}
	//! Функция возвращает метку с названием переменной. \details Читает защищенное поле label_. Метка не может быть изменена.
	const QLabel* getLabel() const {return label_;}
	//! Функция задает текстовый редактор для значения переменной. \details Изменяет защищенное поле editor_.
	void setLineEditor(QLineEdit* lineedit) {editor_ = lineedit;}
	//! Функция возвращает указатель на текстовый редактор со значением переменной. \details Читает защищенное поле editor_. Текстовый редактор может быть изменен.
	QLineEdit* getLineEditor() {return editor_;}
	//! Функция возвращает указатель на текстовый редактор со значением переменной. \details Читает защищенное поле editor_. Текстовый редактор не может быть изменен.
	const QLineEdit* getLineEditor() const {return editor_;}

	//! Печатает глобальную переменную в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::GlobalValuePrintElem).
	ostream& printElem(ostream& s) const;
};

/*! \brief Класс для печати глобальных переменных в стиле оформления FileToTabWidget
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * В отличие от класса fttw::GlobalValuePrintElem, позволяет печатать сразу несколько
 * глобальных переменных.
 * Данный класс применяется тогда, когда в .in-файле в одной строке задано сразу
 * несколько переменных.
 *
 * Хранит в себе массив меток с названиями переменных и массив меток с текстовыми
 * редакторами, которые содержат значения переменных.
 * Переменной с именем в labels_[i] соответствует текстовый редактор со значением
 * в editors_[i].
 *
 * Выводит глобальные переменные в виде:
 *
 * #head NameOfValue1 NameOfValue2 NameOfValue3
 *
 * Value1 Value2 Value3
 *
 *
 * пример:
 *
 * #head T dt A
 *
 * 1000 0.01 12321.323
 *
 *
 * Схема использования:
 * \code
 *
 * //создаем метки и поля для редактирования со значением
 * QLabel *label1 = new QLabel("T");
 * QLineEdit *le1 = new QLineEdit("1000");
 *
 * QLabel *label2 = new QLabel("dt");
 * QLineEdit *le2 = new QLineEdit("0.01");
 *
 * QLabel *label3 = new QLabel("A");
 * QLineEdit *le3 = new QLineEdit("12321.323");
 *
 * //создаем элемент данного класса
 * fttw::GlobalValuesPrintElem glv = new fttw::GlobalValuesPrintElem;
 *
 * glv -> addLabel(label1);
 * glv -> addEditor(le1);
 *
 * glv -> addLabel(label2);
 * glv -> addEditor(le2);
 *
 * glv -> addLabel(label3);
 * glv -> addEditor(le3);
 *
 * //вывод, например, в стандартный поток вывода
 * glv -> printElem(std::cout);
 *
 * delete glv;
 *
 * \endcode
 *
 */
class GlobalValuesPrintElem : public PrintElement{
	//! Массив, содержащий указатели на метки с названиями переменных.
	vector<QLabel*> labels_;
	//! Массив, содержащий указатели на текстовые редакторы со значениями переменных.
	vector<QLineEdit*> editors_;

public:
	//! Пустой конструктор. Все элементы должны быть добавлены с помощью функций addLabel() и addEditor().
	GlobalValuesPrintElem() : PrintElement() {}
	//! Деструктор.
	~GlobalValuesPrintElem() {}

	//! Функция добавляет метку с названием переменной в конец массива указателей на метки с названиями переменных. \details Изменяет защищенное поле labels_ (добавляет элемент в конец).
	void addLabel(QLabel* label) {labels_.push_back(label);}
	//! Функция возвращает метку с названием переменной из массива по индексу ind. \details Читает элемент защищенного поля labels_. Элемент может быть изменен.
	QLabel* getLabel(size_t ind) {return labels_[ind];}
	//! Функция возвращает метку с названием переменной из массива по индексу ind. \details Читает элемент защищенного поля labels_. Элемент не может быть изменен.
	const QLabel* getLabel(size_t ind) const {return labels_[ind];}

	//! Функция добавляет текстовый редактор со значением переменной в конец массива указателей на текстовые редакторы со значениями переменных. \details Изменяет защищенное поле editors_ (добавляет элемент в конец).
	void addEditor(QLineEdit* editor) {editors_.push_back(editor);}
	//! Функция возвращает текстовый редактор со значением переменной из массива по индексу ind. \details Читает элемент защищенного поля editors_. Элемент может быть изменен.
	QLineEdit* getEditor(size_t ind) {return editors_[ind];}
	//! Функция возвращает текстовый редактор со значением переменной из массива по индексу ind. \details Читает элемент защищенного поля editors_. Элемент не может быть изменен.
	const QLineEdit* getEditor(size_t ind) const{return editors_[ind];}

	//! Печатает глобальные переменные в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::GlobalValuesPrintElem).
	ostream& printElem(ostream &s) const;
};


/*! \brief Класс для печати комментариев в стиле оформления FileToTabWidget
 *
 * Наследуется от класса fttw::PrintElement.
 *
 * Печатает комментарий в указанный поток вывода.
 *
 * Хранит в себе только строку, содержащую комментарий. Комментарием также
 * может быть пустая строка - тогда просто выведется знак перевода строки.
 *
 * Выводит комментарий просто строкой, после которой следует символ перевода строки.
 *
 * Схема использования:
 * \code
 *
 * CommentElement *ce = new CommentElement("#this is my comment");
 * ce -> printElem();
 *
 * \endcode
 *
 */
class CommentElement : public PrintElement{
	//! Строка, содержащая комментарий.
	string str_;

public:
	//! Пустой конструктор. Задает поле str_ равным "".
	CommentElement() : PrintElement(), str_("") {}
	//! Конструктор с параметром.
	explicit CommentElement(const string& str) : PrintElement(), str_(str) {}
	//! Деструктор.
	~CommentElement() {}

	//! Функция задает строку, содержащую комментарий. \details Изменяет защищенное поле str_. Версия функции для обычного класса std::string.
	void setComment(const string& str) {str_ = str;}
	//! Функция задает строку, содержащую комментарий. \details Изменяет защищенное поле str_. Версия функции для QString.
	void setComment(const QString& str) {str_ = str.toStdString();}
	//! Функция возвращает строку, содержащую комментарий. \details Читает защищенное поле str_. Строка не может быть изменена.
	const string& getComment() const {return str_;}

	//! Печатает комментарий в стиле файла для fttw::FileToTabWidget (см. описание класса fttw::CommentElement).
	ostream& printElem(ostream &s) const;
};

}

#endif // PRINTELEMENT13022017GASPARYANMOSES_H

/*@}*/
