/** @addtogroup Notepad
 * @{*/

/* @file */

#ifndef NOTEPADWIDGET_H
#define NOTEPADWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QLabel>
#include <QTextDocument>
#include <QTextStream>
#include <QSyntaxHighlighter>
#include <QTextCursor>


 /*!
 *  \~russian \brief Отвечает за синтаксическую подсветку тескта в поле виджета NotepadWidget, согласно заранее предусмотренных регулярным выражениям.
 *
 *  
 */
 
class Highlighter : public  QSyntaxHighlighter
{
public:
    //! \~russian Конструктор класса. \details \~russian
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat multiLineCommentFormat;

    QTextCharFormat paradigmaNeursFormat;
    QTextCharFormat paradigmaNeursParamsFormat;
    QTextCharFormat floatFormat;
    QTextCharFormat integerFormat;
};

 /*!
 *  \~russian \brief Вспомогательный класс, используемый для созлания диалогового окна
 *
 */
 
class UserDialog: public QDialog {
    Q_OBJECT
public:
    UserDialog();
    QLabel*      dialogLabel;
    QLineEdit*   dialogLineEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QVBoxLayout* dialogMainLayout;
    QHBoxLayout* buttonsLayout;
};


/*!
 *  \~russian \brief Виджет представляющий элемент графического интерфейса "Редактор"собой. Явялется составялющей частью копоненты 3d Атласа "Симулятор" и "Интегратор"
 *
 *  \~russian Располагет в своей рабочей области содержимое конфигурационного(текстового) файла с синтаксической подсветкой. Позволяет создавать новые файлы, сохранять их, редактировать.
 *  Синтаксическая подсветка осуществлена классом Highlighter, являющимся наследником QSyntaxHighlighter.
 *  
 */
 
 
class NotepadWidget: public QWidget {
    Q_OBJECT
private:
    QTextEdit* txtField;
    QPushButton* openButton;
    QPushButton* saveButton;
    QPushButton* saveAsButton;
    QPushButton* exportButton;
    QPushButton* newFileButton;
    QPushButton* closeButton;
    QVBoxLayout* buttonsRightLayout;
    QVBoxLayout* buttonsLeftLayout;
    QHBoxLayout* mainLayout;
    QString      text;
    QFileDialog* openFileDialog;
    QFile        file;
    UserDialog* userDialog;
    QString     fileName;
    Highlighter* highlighter;

public:
    NotepadWidget(QWidget* pwgt = 0);
    //QLabel*     paradigmaLabel;
    //QLabel*     integerValueParadigm;
    //QLabel*     floatValueParadigm;
    
    //! \~russian Метод отвечающий за чтение  файла конфигурации  \~russian
    void        readFile();

signals:
    //! \~russian Сигнал необходимый за экспорт имени файла конфигурации открытого в NotepadWidget \~russian
    void setFileName(QString filename);

public slots:
    //! \~russian Метод отвечающий за открытие нового файла конфигурации  в NotepadWidget \~russian
    void openFile();
    //! \~russian Метод отвечающий за окрытие файла конфигурации  в NotepadWidget \~russian
    void openFile(QString filename);
    //! \~russian Метод отвечающий за выбор файла конфигурации  в диалоговом окне  \~russian
    void openFileSlot();
    //! \~russian Метод отвечающий за сохранение файла конфигурации \~russian
    void saveFile();
    //! \~russian Вспомогателный метод, отвечающий за сохранение файла конфигурации \~russian
    void saveFileSlot();
    //! \~russian Метод отвечающий за сохранение файла конфигурации под новым именем \~russian
    void saveFileAs();
    //! \~russian Метод отвечающий за отображение  вспомогательного диалогового окна \~russian    
    void newFileDialog();
    //! \~russian Метод отвечающий за отображения диалога создания нового файла конфигурации \~russian    
    void newFile();
    //! \~russian Метод необходимый за экспорт имени файла конфигурации открытого в NotepadWidget \~russian        
    void exportFile();
    //! \~russian Метод отвечающий за создание нового файла конфигурации \~russian        
    void createNewFile();
    //! \~russian Метод отвечающий за активацию кнопки "Сохранить"\~russian        
    void enableSaveButton();
    //! \~russian Метод отвечающий за деактивацию кнопки "Сохранить"\~russian            
    void disableSaveButton();
};

#endif // NOTEPADWIDGET_H

/*@}*/
