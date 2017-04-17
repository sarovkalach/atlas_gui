#ifndef INTEGRATORWIDGET_GASPARYANMOSES_11042017
#define INTEGRATORWIDGET_GASPARYANMOSES_11042017

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QLabel>
#include <QProcess>
#include <QTextEdit>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QFileDialog>
#include <QString>

#include <vector>

#include "../general_gui_files/include/shell.h"
#include "../general_gui_files/include/textdialog.h"
#include "integratorsshworker.h"
#include "tableintegrelem.h"
using namespace std;

/*! \brief Главный виджет интегратора.
 *
 * Является главным виджетом компонента "Интегратор".
 * Содержит все необходимые кнопки, модель-представление
 * таблицы atlas.integrate .
 *
 * Содержит в себе объект для запуска конвертера.
 *
 * Использовать как обычный виджет.
 *
 * Схема использования:
 * \code
 *
 * int owner = 4;
 * IntegratorWidget wid(owner);
 * wid.show();
 *
 * \endcode
 *
 */
class IntegratorWidget : public QWidget{
	Q_OBJECT
private:
	//! Идентификационный номер пользователя. Устанавливается в конструкторе.
	int owner_;

	//! Главный менеджер размещения для данного виджета.
	QVBoxLayout* mL_;
	//! Метка "Set converter:" .
	QLabel* setIntegratorLabel_;
	//! Выпадающий список со всеми интеграторами из таблицы atlas.integrator в базе данных.
	QComboBox* integratorsCB_;
	//! Неизменяемое текстовое поле с описанием выбранного в выпадающем списке конвертера.
	QTextEdit* integratorDescription_;

	//! Блокнот для изменения файлов.
	SimpleArtificialShell* notepad_;

	//! Менеджер размещения кнопок.
	QHBoxLayout* buttonLayout_;
	//! Кнопка запуска конвертера. \details Соединена со слотом startIntegrate() .
	QPushButton* startButton_;
	//! Кнопка удаления записи из базы данных в таблице atlas.integrate , а также удаление .out файлов, связанных с работой этой программы. \details Соединена со слотом deleteRecords() .
	QPushButton* deleteButton_;
	//! Кнопка открывает диалоговое окно, в котором есть тестовый редактор, куда можно вписать комментарий для запуска. \details Соединена со слотом addComment() .
	QPushButton* commentButton_;
	//! Кнопка позволяет открыть диалоговое окно для выбора входного(-ых) файла(-ов). \details Соединена со слотом chooseInFiles() .
	QPushButton* chooseInFilesButton_;
	//! Кнопка для открытия блокнота, в котором можно редактировать файлы. \details Соединена со слотом showNotepad() .
	QPushButton* notepadButton_;

	//! Модель для таблицы atlas.integrate из базы данных.
	QSqlRelationalTableModel* integrateDataModel_;
	//! Представление таблицы atlas.integrate из базы данных.
	QTableView* integrateDataView_;

	//! Объект, который позволяет запускать конвертер, добавлять запись о запуске в таблицу atlas.integrate .
	IntegratorSSHWorker puskach_;
	//! Массив всех интеграторов, считанные из таблицы atlas.integrator .
	vector<TableIntegrElem> allIntegrators_;
	//! Каталог, где хранятся все входные файлы, а также куда будут выводиться выходные файлы. Путь относительно локального компьютера.
	QString catalog_;

	//! Функция заполняет массив всех интеграторов из таблицы atlas.integrator из базы данных. Данная функция должна быть вызвана лишь один раз в конструкторе.
	void initAllIntegrators();
	//! Функция создает все кнопки данного виджета, соединяет со всеми слотами, а также добавляет их в менеджер размещения кнопок. Данная функция должна быть вызвана лишь один раз в конструкторе.
	void initAllButtons();
	//! Функция создает модель-представление для таблицы atlas.integrate из базы данных. Определяет столбцы, которые будут видны пользователю. Данная функция должна быть вызвана лишь один раз в конструкторе.
	void initMainTable();
	//! Функция инициализации. Вызывает все функции инициализации, а также доопределяет оставшиеся единичные объекты, соединяет все элементы в главном менеджере размещения. Данная функция должна быть вызвана лишь один раз в конструкторе.
	void init();

	//! Функция устанавливает связь модели-представления таблицы atlas.integrate с таблицой atlas.integrator, чтобы вместо выбранного идентификационного номера конвертера показывать его название. Функция должна быть обязательно вызвана, если изменяется идентификационный номер пользователя.
	void setAtlasIntegratorRelations();

public:
	//! Конструктор. Принимает идентификационный номер пользователя.
	explicit IntegratorWidget(int owner, QWidget* pwg = Q_NULLPTR);
	//! Деструктор.
	~IntegratorWidget() {}

	//! Функция обновляет модель-представление из БД atlas.integrate . Если входной идентификационный номер пользователя не совпадает с тем, который есть в классе, то делается также заново привязка к таблице atlas.integrate .
	void showIntegratetableModel(int owner);

	//! Функция возвращает идентификационный номер текущего пользователя. \details Читает приватное поле owner_.
	const int& getCurrentOwner() const { return owner_; }
	//! Функция устанавливает идентификационный номер пользователя. \details Изменяет приватное поле owner_, а также меняет модель-представление .
	void setOwner(int owner) { showIntegratetableModel(owner); }
	//! Функция возвращает массив всех интеграторов, которые были считаны из таблицы atlas.integrator . \details Читает приватное поле allIntegrators_ .
	const vector<TableIntegrElem>& getAllIntegrators() const { return allIntegrators_; }
	//! Функция возвращает выбранный каталог, где хранятся все входные файлы, а также куда будут выводиться выходные файлы. \details Читает приватное поле catalog_ .
	const QString& getCatalog() const { return catalog_; }

private slots:
	//! Слот запускает работу конвертера на сервере, добавляет запись о запуске в БД.
	void startIntegrate();
	//! Слот удаляет выделенные в таблице записи из БД, а также трет все файлы, которые являются выходными для запусков. ВНИМАНИЕ: если имена выходных файлов для выделенного запуска совпадают с именами выходных файлов для других запусков, то файл все равно будет удален!
	void deleteRecords();
	//! Слот показывает блокнот для редактирования файлов.
	void showNotepad();
	//! Слот позволяет показать или скрыть описание интегратора. Если описание отсутствует, то integratorDescription_ скрывается.
	void showAnotherDescription(int ind);
	//! Слот открывает диалоговое окно для добавления/редактирования комментария к будущему запуску конвертера.
	void addComment();
	//! Слот открывает диалоговое окно для выбора входных файлов.
	void chooseInFiles();

};

#endif // INTEGRATORWIDGET_GASPARYANMOSES_11042017
