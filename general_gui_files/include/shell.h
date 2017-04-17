/** @addtogroup oscillators
 * @{*/

/** @file */

#ifndef SIMPLEARTIFICIALSHELL200217_GASPARYANMOSES
#define SIMPLEARTIFICIALSHELL200217_GASPARYANMOSES

#include "filetotabwidget.h"
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QLineEdit>

/*! \brief Виджет для открытия файла в специальном редакторе.
 *
 * Виджет с кнопкой для открытия диалогового окна,
 * которое позволяет выбрать in-файл для специального
 * текстового редактора.
 * Текстовый редактор открывает файл в соответствии с метками:
 *
 * либо как файл с таблицами и вкладками, либо как обычный
 * текстовый редактор, но с подсветкой синтаксиса (подробности можно
 * найти в описании класса fttw::FileToTabWidget).
 *
 * Класс содержит слоты для открытия файла в специальном текстовом редакторе.
 *
 *
 * Схема использования:
 * \code
 *
 * 	SimpleArtificialShell* mW = new SimpleArtificialShell;
 *	mW->show();
 *
 * \endcode
 *
 */
class SimpleArtificialShell : public QWidget{
	Q_OBJECT
private:
	//! Главный виджет с текстовым редактором и его кнопками.
	fttw::OutSideWidget *fileToTab_ {nullptr};
	//! Главный менеджер расстановки для данного виджета.
	QVBoxLayout *mL_;
	//! Менеджер расстановки для текстового редактора (название входного файла) и кнопки.
	QHBoxLayout *buttonLayout_;
	//! Кнопка для открытия файла.
	QPushButton *openFileButton_;
    //! Текстовый редактор с именем файла, который сейчас открыт в специальном текстовом редакторе.
    QLineEdit* inFileLab_;
	//! Папка по умолчанию, которая открывается перед пользователем при нажатии кнопки "Open in-file". Устанавливается внешним виджетом, если нужно.
	QString defaultDir_;

	//! Проверяет, существует ли файл. Если не существует, то создает новый пустой файл. Если файл создать не удалось, то возвращает false. Во всех остальных случаях возвращает true.
	bool checkExistenceAndCreateIfNot(const QString& filename) const;

public:
	//! Конструктор.
    explicit SimpleArtificialShell(QWidget* obj = nullptr);
	//! Деструктор.
	~SimpleArtificialShell() {}


	//! Возвращает главный виджет с текстовым редактором и его кнопками. \details Читает защищенное поле fileToTab_. fileToTab_ можно менять.
	fttw::OutSideWidget* get_OutsideFileToTabWidget() {return fileToTab_;}
	//! Возвращает главный виджет с текстовым редактором и его кнопками. \details Читает защищенное поле fileToTab_. fileToTab_ нельзя менять.
	const fttw::OutSideWidget* get_OutsideFileToTabWidget() const {return fileToTab_;}

	//! Возвращает кнопку для открывания диалогового окна выбора файла. \details Читает защищенное поле openFileButton_. openFileButton_ можно менять.
	QPushButton* get_openFileButton() {return openFileButton_;}
	//! Возвращает кнопку для открывания диалогового окна выбора файла. \details Читает защищенное поле openFileButton_. openFileButton_ нельзя менять.
	const QPushButton* get_openFileButton() const {return openFileButton_;}

	//! Возвращает имя in-файла, который сейчас открыт в текстовом редакторе. \details Читает часть приватного поля inFileLab_.
	QString get_inFileName() const {return inFileLab_->text();}
	//! Возвращает текстовый редактор с именем файла, который открыт сейчас в специальном текстовом редакторе. \details Читает приватное поле inFileLab_. inFileLab_ можно менять.
	QLineEdit* get_inFileLineEditor() {return inFileLab_;}
	//! Возвращает текстовый редактор с именем файла, который открыт сейчас в специальном текстовом редакторе. \details Читает приватное поле inFileLab_. inFileLab_ нельзя менять.
	const QLineEdit* get_inFileLineEditor() const {return inFileLab_;}

	//! Устанавливает имя папки по умолчанию, открывающейся перед пользователем при нажатии кнопки "Open in-file". \details Изменяет приватное поле defaultDir_.
	void set_defaultDir(const QString& dirIn) { defaultDir_ = dirIn; }
	//! Возвращает имя папки по умолчанию, открывающейся перед пользователем при нажатии кнопки "Open in-file". \details Читает приватное поле defaultDir_.
	const QString& get_defaultDir() const { return defaultDir_; }

private slots:
	//! Открывает указанный файл с помощью редактора (таблицы/вкладки или текстовый редактор). Не меняет inFileLab_. Вызывается сигналом изменения текста в inFileLab_.
	void openFileForEditing(QString str);


public slots:
	//! Создает диалоговое окно выбора файла, а затем открывает файл в специальном текстовом редакторе (вызывает метод openFileForEditing через сигнал).
	void chooseFileForEditing();
	//! Сравнивает имя файла в inFileLab_ и str. Если не совпадает, то заменяет текущее имя in-файла на str.
	void openFileForEditingWithLineEditor(const QString& str);


};

#endif // SIMPLEARTIFICIALSHELL200217_GASPARYANMOSES

/*@}*/
