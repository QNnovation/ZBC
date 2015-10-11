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
    void viewFile(const QString &);
    void editFile(const QString &);
    ~wgtTextView();

protected:
    void closeEvent(QCloseEvent *);

private:
    bool             m_rwMode;

    QMenu           *m_menu;
    QMenu           *m_editMenu;

    QAction         *m_quitAct;
    QAction         *m_saveAct;
    QAction         *m_fileSaveAsAct;
    QAction         *m_findAtTextAct;
    QAction         *m_undoAct;
    QAction         *m_redoAct;

    void createMenu();
    void createActions();

    QPlainTextEdit  *m_textView;

    QString pathToFile;
    FindReplaceText *m_findReplace;
    void replace(const QString &, const QString &, QTextDocument::FindFlags);
    void replaceAll(const QString &, const QString &, QTextDocument::FindFlags);

private slots:
    bool saveAs();
    bool saveFile();

    //slots for wgtfindreplacetext
    void find();
    void findSlot(const QString &, QTextDocument::FindFlags);
    void replaceSlot(const QString &, QTextDocument::FindFlags, const QString &);
    void replaceAllSlot(const QString &, QTextDocument::FindFlags, const QString &);

};

#endif // WGTTEXTVIEW_H
