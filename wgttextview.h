#ifndef WGTTEXTVIEW_H
#define WGTTEXTVIEW_H

#include <QMainWindow>
class QString;
class QPlainTextEdit;
class QMenu;
class QToolBar;
class QAction;

class wgtTextView : public QMainWindow
{
    Q_OBJECT

public:
    wgtTextView();
    void viewFile(const QString&);
    void editFile(const QString&);

private slots:
    void open();
    void newFile();
    bool save();
    bool saveAs();
    void documentWasModified();

private:
    void loadFile(const QString&);
    bool saveFile(const QString&);
    void setCurrentFile(const QString&);
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    bool maybeSave();

    QPlainTextEdit *textView;
    QString currentFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *exitAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *cutAct;

};

#endif // WGTTEXTVIEW_H
