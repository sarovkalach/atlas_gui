#ifndef TEXTDIALOG_GASPARYANMOSES11042017
#define TEXTDIALOG_GASPARYANMOSES11042017

#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QDialog>

/*! \brief Диалоговое окно текстового редактора.
 *
 * Вызывает диалоговое окно с текстовым редактором (QTextEdit),
 * в котором изначально написан текст, который поступил в качестве
 * аргумента. Его же диалоговое окно и меняет при нажатии на кнопку "OK".
 *
 * Используется внутри SimulatorWidget для добавления описания.
 *
 * Схема использования:
 * \code
 *
 * QString test = "Hello!";
 * TextDialog::createTextDialog(QString& text);
 * qDebug()<<"user entered:";
 * qDebug()<<text;
 *
 * \endcode
 *
 */
class TextDialog : public QDialog{
	Q_OBJECT
protected:
	//! Текст, который будет изменен при работе данного виджета. Также является входным параметром.
	QString& text_;
	//! Главный менеджер размещения.
	QVBoxLayout* mL_;
	//! Текстовый редактор
	QTextEdit* editor_;
	//! Кнопка "Ок". \details Соединена со слотом pushOk() .
	QPushButton* okButton_;

	//! Конструктор.
	explicit TextDialog(QString& text, QWidget* pwg = Q_NULLPTR);
	//! Деструктор.
	~TextDialog() {}

public:
	//! Статическая функция для создания данного диалогового окна.
	static void createTextDialog(QString& text, QWidget* pwg = Q_NULLPTR){
		TextDialog tdial(text, pwg);
		tdial.exec();
	}

private slots:
	//! Слот, который сохраняет введенный в текстовом редакторе текст во входную переменную text_, закрывает диалоговое окно.
	void pushOk();
};

#endif // TEXTDIALOG_GASPARYANMOSES11042017
