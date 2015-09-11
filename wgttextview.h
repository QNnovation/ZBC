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
    ~wgtTextView();
    bool loadFile(const QString &, char mode = 'r');
    void viewFile(QString &);
    void editFile(QString &);

protected:
    void closeEvent(QCloseEvent *);

private:
    bool rwMode;

    QMenu *menu;
    QMenu *editMenu;

    QAction *quitAct;
    QAction *fileSaveAsAct;
    QAction *findAtTextAct;

    void createMenu();
    void createActions();

    QPlainTextEdit *textView;

    QString pathToFile;
    FindReplaceText *findReplace;

private slots:
    bool saveAs();
    bool saveFile();
    void findTextOptionsSlot(QString, QTextDocument::FindFlags);
    void findInTextSlot();


};

#endif // WGTTEXTVIEW_H
