/** @addtogroup oscillators
 * @{*/

/** @file */

#ifndef FILETOTABWIDGET16012017GASPARYANMOSES_H
#define FILETOTABWIDGET16012017GASPARYANMOSES_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QTextEdit>
#include <QTableWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QItemDelegate>
#include <QHeaderView>
#include <QSyntaxHighlighter>
#include <QPushButton>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <iterator>

#include "printelement.h"
using namespace std;

namespace fttw{

//! Преобразует элемент на входе в строку. У элемента должен быть определен оператор вывода.
template<typename T>
string toStr(T elem);

//! Печатает в поток ошибок сообщение об ошибке с указанием функции, в которой произошла ошибка и номер строки, на которой произошла ошибка, а затем выходит из программы с помощью exit(-1)
void print_mistake(const string& funcName, int line, const string& fileName, const string& message);
//! Печатает в поток ошибок сообщение об ошибке с указанием функции, в которой произошла ошибка и номер строки, на которой произошла ошибка, НЕ выходит из программы.
void print_mistakeLite(const string& funcName, int line, const string& fileName, const string& message);

//! Считывает из строчного потока до символа '\n' не включая и возвращает строку.
string getOneLine(istream& ss);
//! Считывает из строчного потока до символа '\n' не включая и записывает результат в vs. Возвращает true, если поток не закончился
bool getOneLine(istream& ss, string& vs);

//! Проверяет, является ли строка вещественным числом.
inline bool isDouble(const string& s);

//!	Шаблонная функция. Преобразует строку в указанный тип.
template<typename T>
T fromStringTo(const string& s);

//! Печатает имя родителя объекта obj, если он существует. Если нет - выводит "nullptr".
inline ostream& print_parent(ostream& s, const QObject* obj);
//! Печатает имя объекта obj, если он существует. Если нет - выводит "nullptr". также печатает имя родителя obj, если это возможно.
inline ostream& tryToPrint(ostream& s, const QObject* obj);



/*! \brief Вспомогательный класс, позволяющий вводить в таблицу только double.
 *
 * Позволяет вводить в ячейку таблицы только double от его минимального до
 * его максимального значения с точностью до 15-ти знаков после запятой.
 *
 * Схема использования:
 * \code
 *
 *  int Nrow = 5;
 *  int Ncol = 3;
 *
 * 	QTableWidget* table = new QTableWidget(Nrow, Ncol);
 *	table->setItemDelegate(new OnlyDoubleDelegate);
 *  //далее заполняем таблицу числами как обычно
 *  //...
 *
 * \endcode
 *
 */
class OnlyDoubleDelegate : public QItemDelegate{
	Q_OBJECT
public:
	//! Отнаследованная от QItemDelegate виртуальная функция. Создана для того, чтобы нельзя было вводить в текстовое поле не вещественное число.
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};




/*! \brief Вспомогательный класс для выделения
 *
 * Позволяет выделять синим все ключевые слова, которые относятся к разметке документа,
 * темно-красным все целые числа(int), красным - все вещественные числа (double).
 *
 * Схема использования:
 * \code
 *
 * QTextEdit *te = new QTextEdit;
 * HighLightNumbers = new HighLightNumbers(te);
 * //теперь все символы будут подсвечены согласно выделителю.
 * //...
 *
 * \endcode
 *
 */
class HighLightNumbers : public QSyntaxHighlighter{
	Q_OBJECT
private:
	//! Струкрута, которая содержит в себе регулярное выражение и формат для всех слов, которые этому регулярному выражению соответствуют. В нашей программе используется только цвет слов из всех возможностей формата.
	struct HighlightRule{
		QRegExp pattern;
		QTextCharFormat format;
	};

	//! Формат для всех вещественных чисел (красный цвет).
	QTextCharFormat doubleFormat_;
	//! Формат для всех целых чисел (темно-красный цвет).
	QTextCharFormat intFormat_;
	//! Формат для всех ключевых слов, которые участвуют в разметке текста (синий).
	QTextCharFormat keywordFormat_;
	//! Вектор по всеми правилами форматирования. Создается в конструкторе и больше нигде не меняется.
	QVector<HighlightRule> rules_;

	//! Функция инициализации. Вызывается только в конструкторе и только один раз.
	void init();

public:
	//! Пустой конструктор.
	explicit HighLightNumbers(QObject* pwg = nullptr) : QSyntaxHighlighter(pwg){init();}
	//! Конструктор с текстовым документом. Делает текстовый документ pwg родителем объекта данного класса, а также выполняет для pwg функцию highlightBlock(const QString&).
	explicit HighLightNumbers(QTextDocument* pwg) : QSyntaxHighlighter(pwg){init();}
	//! Виртуальная функция, в которой применяются правила выделения текста rules_ к одной строке str.
	virtual void highlightBlock(const QString &str);
};



/*! \brief Класс, преобразующий файл в виджет и обратно.
 *
 * Объект данного класса может создать виджет на основе текстового файла,
 * оформленного определенным образом. После внесенных изменений может
 * сохранить данные в формате, который можно будет считать этой же программой.
 *
 * Если текстовый документ оформлен некорректно, то открывается обычный
 * текстовый редактор.
 *
 * Имеет несколько слотов для загрузки, сохранения, очистки.
 *
 * Схема использования:
 * \code
 *
 *	string filename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/infile.in";
 *
 *	fttw::FileToTabWidget mW(filename);
 *	mW.show();
 *
 * \endcode
 *
 */
class FileToTabWidget : public QScrollArea{
	Q_OBJECT
private:
	//! Глобальные переменные. Отображаются как Label    value. В текстовом файле отмечаются как: #head название_переменной, на следующей строке - значение (#head T, 100).
	vector<pair<QLabel*, QLineEdit*> > globalValues_;
	//! Таблицы с данными. Каждый #data - в файле - это новая таблица (#data Va Vb Vc, 1 2 3, 4 5 6, 7 8 9).
	vector<QTableWidget*> tables_;
	//! Главный виджет, на который все накладывается.
	QFrame *mW_ {nullptr};
	//! Менеджер размещения для главного виджета mW_.
	QBoxLayout *mL_ {nullptr};
	//! Вкладки с таблицами. Каждая таблица - новая вкладка. Вкладка может иметь название, которое задается с помощью ключевого слова #dataname (#dataname name, #data Va Vb Vc, 1 2 3, 4 5 6, 7 8 9).
	QTabWidget *tabs_ {nullptr};
	//! Текстовый редактор. Используется, если файл на входе некорректно размечен.
	QTextEdit* unmarkedValues_ {nullptr};
	//! Специальный выделитель слов в тексте для текстового редактора QTextEdit.
	HighLightNumbers* textEditorHighLighter_ {nullptr};
	//! Массив с элементами, у которых вызывается функция для печати. В зависимости от внутренней реализации элемента, печать происходит по-разному. Все таблицы, глобальные переменные, комментарии записываются в этот массив.
	vector<PrintElement*> printElements_;

public:
	//! Тип поведения. Tabs - будет сделана попытка по текстовому файлу создать виджет с таблицами и окошками для глобальных переменных. TextEditor - по файлу будет создан текстовый редактор.
	enum mode {Tabs, TextEditor};
private:
	//! Значение типа поведения. Может быть задано в конструкторе.
	mode mode_;

	//! Инициализация менеджера размещения. Создает главный менеджер размещения и прикрепляет его к главному виджету. Используется лишь один раз в функции main_init(). \details Изменяет защищенные поля mL_ и mW_.
	inline void layout_init();
	//! Функция открывает файл filename и считывает всю информацию из него в строковый буфер s.
	void createFileBuffer(const string& filename, stringstream& s) const;
	//! Функция делает из строки с метками один вектор, где каждым элементом является метка.
	void createLabelVector(const string& labels, vector<string>& vec) const;
	//! Считывает глобальные параметры с именами в строке labels. Строка уже не должна содержать "#head". Возвращает количество созданных глобальных переменных. \details Изменяет защищенные поля globalValues_, mW_, mL_.
	size_t readGlobalValues(istream& ss, const string& labels);
	//! Считывает таблицу с данными. Строка уже не должна содержать "#data". Считывает до тех пор, пока подряд идут вещественные числа. Возвращает количество пустых строк после таблицы. \details Изменяет защищенные поля tables_, mW_, mL_.
	size_t readTable(istream& ss, const string& labels);
	//! Создает под уже оформленную таблицу специальную отдельную вкладку в виджете со вкладками. Название вкладки - ее номер. Если виджет со вкладками ещё не был создан, то создает его. \details Изменяет защищенное поле tabs_.
	void addTableToTab(QTableWidget* table);
	//! Перегруженная функция. Создает под уже оформленную таблицу специальную отдельную вкладку в виджете со вкладками. Название вкладки - tabname. Если виджет со вкладками ещё не был создан, то создает его. \details Изменяет защищенное поле tabs_.
	void addTableToTab(QTableWidget *table, const QString& tabname);
	//! Получает из потока имя вкладки (все, что после #dataname, является именем вкладки, включая пробелы, кроме самого первого).
	QString getTabName(istream& ss) const;
	//! Выполняет сохранение файла из текстового редактора QTextEdit, а точнее, из unmarkedValues_. Используется только в режиме TextEditor. \details Является вспомогательной функцией save_file(const string&).
	void saveFromEditor(ostream& fout) const;
	//! Выполняет сохранение файла из созданных объектов. Используется только в режиме Tabs. \details Вызывает функции printElem(ostream&) из массива printElements_.
	void saveFromData(ostream& fout) const;
	//! Устанавливает ширину главного окна по ширине самой широкой таблицы. \details Изменяет размер данного объекта.
	void setWidthFromTables();
	//! Данная функция используется только один раз в конструкторе. Создает главный виджет mW_, менеджер размещения mL_, прикрепляет все к основному виджету прокрутки. Также задает некоторые флаги виджета прокрутки.
	void main_init();
	//! Функция для загрузки файла filename в данный виджет. \details Затрагивает все параметры.
	void load_file(const string& filename);
	//! Проверка данных в строке на достаточное количество введенных значений для глобальных переменных с названиями в labels. Возвращает true, если все введено корректно.
	bool checkGlobalValues(istream& ss, const string& labels) const;
	//! Проверка данных в строке на корректный ввод элементов одной таблицы с количеством столбцов из labels. Возвращает true, если все введено корректно.
	bool checkTable(istream& ss, const string& labels) const;
	//! Функция для проверки корректности всех строк файла с именем filename. Если данные введены некорректно, то режим переключается в TextEditor.
	bool check_correct(const string& filename) const;
	//! Создает виджет с таблицами и полями из файла с именем filename. Данная функция вызывается уже после проверки файла функцией check_correct(const string&). \details Изменяет все параметры, кроме unmarkedValues_.
	void createTabsAndGlobalValues(const string &filename);
	//! Создает текстовый редактор из файла с именем filename. Вызывается, если данные файла были введены некорректно или если выставлен режим TextEditor. \details Изменяет защищенное поле unmarkedValues_.
	void createTextEditor(const string& filename);

public:
	//! Конструктор с именем файла. После основных инициализаций вызывает метод load_file(const string&).
	explicit FileToTabWidget(const string& filename, QWidget* pwg = nullptr, mode in_mode = Tabs);
	//! Деструктор.
	~FileToTabWidget() {
		for(size_t i=0; i<printElements_.size(); ++i){
			delete printElements_[i];
		}
	}

	//! Функция для печати всех элементов данного виджета в поток. Используется для отладки.
	ostream& print(ostream& s) const;
	//! Функция читает защищенное поле режима работы виджета.
	mode getMode() const {return mode_;}
	//! Возвращает главный виджет, на который все накладывается. \details Читает приватное поле mW_. mW_ может быть изменено.
	QFrame* get_mainFrame() {return mW_;}
	//! Возвращает главный виджет, на который все накладывается. \details Читает приватное поле mW_. mW_ не может быть изменено.
	const QFrame* get_mainFrame() const {return mW_;}

public slots:
	//! Сохраняет текущее состояние переменных в файл filename по типу того, что подавался на вход. \details Считывает все параметры и записывает их. Сами параметры не меняются.
	void save_file(const string& filename) const;
};
//! Оператор вывода для класса FileToTabWidget. Вызывает внутри себя метод print(ostream&).
ostream& operator<<(ostream& s, const FileToTabWidget& tw);



/*! \brief Класс для теста объекта класса FileToTabWidget.
 *
 * Создает виджет, на котором присутствуют
 * 1. виджет класса FileToTabWidget
 * 2. кнопка для сохранения данных
 * 3. кнопка для попытки считывания файла ещё раз (позволяет превратить неразмеченный текст в таблицу).
 * 4. В режиме FileToTabWidget::Tabs также присутствует кнопка переключения режима.
 *
 * Схема использования:
 * \code
 *
 * 	QApplication a(argc, argv);
 *
 *	string filename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/infile.in";
 *	fttw::OutSideWidget mW(filename);
 *	mW.show();
 *
 *	return a.exec();
 *
 * \endcode
 *
 * Если необходимо, чтобы имя файла загрузки и имя файла сохранения отличались, то можно сделать так:
 * \code
 *
 *	string saveFilename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/outfile.out";
 *  string loadFilename = "/home/moses/Documents/Qt_workspace/FileToWidget/FileToWidget/infiles/infile.in";
 *  fttw::OutSideWidget mW(saveFilename, loadFilename);
 *	mW.show();
 *
 * \endcode
 *
 */
class OutSideWidget : public QFrame{
	Q_OBJECT
private:
	//! Виджет, который позволяет преобразовать файл в таблицы и окошки для глобальных параметров. В случае некорректной разметки просто выдает текстовый редактор с введенными данными.
	FileToTabWidget *fttw_;
	//! Менеджер размещения данного виджета.
	QVBoxLayout *mL_;
	//! Кнопка, нажав на которую можно сохранить все введенные данные в файл savename_. \details Связывает сигнал нажатия с вызовом слота save_file(const string&) из fttw_.
	QPushButton *saveButton_;
	//! Кнопка, нажав на которую можно перезагрузить данные из файла. \details Пересоздает виджет fttw_, считывая файл заново.
	QPushButton *reloadButton_;
	//! Кнопка переключения просмотра из режима вкладок и таблиц в режим текстового редактора. Появляется только при режиме таблиц и вкладок. \details Сохраняет состояние в savename_. Удаляет текущий виджет и загружает заново в режиме текстового редактора.
	QPushButton *toTextEditorButton_;
	//! Имя файла, в который будут сохраняться данные из виджета fttw_.
	string savename_;
	//! Имя файла, откуда будут загружаться данные для виджета fttw_.
	string loadname_;

	//! Функция инициализации. Вызывается один раз в конструкторе. Затрагивает все параметры.
	void init();

public:
	//! Конструктор. Задает имя файлов для загрузки и для сохранения одинаковыми и равными name.
	explicit OutSideWidget(const string& name, QWidget* pwg = nullptr);
	//! Конструктор. Позволяет задать различные файлы для загрузки и сохранения.
	OutSideWidget(const string& savename, const string& loadname, QWidget *pwg = nullptr);

	//! Возвращает виджет, который позволяет преобразовать файл в таблицы и окошки для глобальных параметров и в случае некорректной разметки просто выдает текстовый редактор с введенными данными. \details Читает приватное поле fttw_. fttw_ может быть изменено.
	FileToTabWidget* get_FileToTabWidget() {return fttw_;}
	//! Возвращает виджет, который позволяет преобразовать файл в таблицы и окошки для глобальных параметров и в случае некорректной разметки просто выдает текстовый редактор с введенными данными. \details Читает приватное поле fttw_. fttw_ не может быть изменено.
	const FileToTabWidget* get_FileToTabWidget() const {return fttw_;}

	//! Возвращает кнопку сохранения данных файла. \details Читает приватное поле saveButton_. saveButton_ может быть изменено.
	QPushButton* get_saveButton() {return saveButton_;}
	//! Возвращает кнопку сохранения данных файла. \details Читает приватное поле saveButton_. saveButton_ не может быть изменено.
	const QPushButton* get_saveButton() const {return saveButton_;}

	//! Возвращает кнопку перезагрузки данных из файла. \details Читает приватное поле reloadButton_. reloadButton_ может быть изменено.
	QPushButton* get_reloadButton() {return reloadButton_;}
	//! Возвращает кнопку перезагрузки данных из файла. \details Читает приватное поле reloadButton_. reloadButton_ не может быть изменено.
	const QPushButton* get_reloadButton() const {return reloadButton_;}

	//! Возвращает кнопку переключения просмотра из режима вкладок и таблиц в режим текстового редактора. \details Читает приватное поле toTextEditorButton_. toTextEditorButton_ может быть изменено.
	QPushButton* get_toTextEditorButton() {return toTextEditorButton_;}
	//! Возвращает кнопку переключения просмотра из режима вкладок и таблиц в режим текстового редактора. \details Читает приватное поле toTextEditorButton_. toTextEditorButton_ не может быть изменено.
	const QPushButton* get_toTextEditorButton() const {return toTextEditorButton_;}

	//! Возвращает имя файла для сохранения данных. \details Читает приватное поле savename_.
	const string& get_saveName() const {return savename_;}
	//! Возвращает имя файла, из которого загружаются данные. \details Читает приватное поле loadname_.
	const string& get_loadName() const {return loadname_;}

public slots:
	//! Слот, который вызывает сигнал savedata(const string&), чтобы можно было вызвать другой слот из класса FileToTabWidget.
	void savedata_slot();
	//! Слот, который удаляет виджет fttw_ и создает его заново из файла с именем loadname_. НЕ СОХРАНЯЕТ ТЕКУЩЕЕ СОСТОЯНИЕ.
	void reload_slot();
	//! Слот, который удаляет виджет fttw_ и создает его заново из файла с именем loadname_ в режиме текстового редактора. Сохраняет состояние.
	void toTextEditor();

signals:
	//! Сигнал, который позволяет вызвать слот из класса FileToTabWidget.
	void savedata(const string& name);
};

}

#endif // FILETOTABWIDGET16012017GASPARYANMOSES_H


/*@}*/
