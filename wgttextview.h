#ifndef WGTTEXTVIEW_H
#define WGTTEXTVIEW_H

#include <QMainWindow>
#include <QTextDocument>
#include "wgtfindreplacetext.h"

class QMenu;
class QToolBar;
class QAction;
class QPlainTextEdit;

class wgtTextView : public QMainWindow
{
    Q_OBJECT

public:
    explicit wgtTextView(QWidget *parent = 0);
    bool loadFile(const QString &, char mode = 'r');
    void viewFile(QString &);
    void editFile(QString &);
    ~wgtTextView();

protected:
    void closeEvent(QCloseEvent *);

private:
    bool rwMode;

    QMenu *menu;
    QMenu *editMenu;

    QAction *quitAct;
    QAction *fileSaveAsAct;
    QAction *findAtTextAct;
    QAction *undoAct;
    QAction *redoAct;

    void createMenu();
    void createActions();

    QPlainTextEdit *textView;

    QString pathToFile;
    FindReplaceText *findReplace;
    void replace(QString, QString, QTextDocument::FindFlags);
    void replaceAll(QString, QString, QTextDocument::FindFlags);

private slots:
    bool saveAs();
    bool saveFile();

    //slots for wgtfindreplacetext
    void find();
    void findSlot(QString, QTextDocument::FindFlags);
    void replaceSlot(QString, QTextDocument::FindFlags, QString);
    void replaceAllSlot(QString, QTextDocument::FindFlags, QString);

};

#endif // WGTTEXTVIEW_H
