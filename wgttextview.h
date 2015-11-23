#ifndef WGTTEXTVIEW_H
#define WGTTEXTVIEW_H

#include <QMainWindow>
#include <QTextDocument>
#include <QPlainTextEdit>
#include <QPrinter>
#include <QSettings>
#include "wgtfindreplacetext.h"

class QMenu;
class QToolBar;
class QAction;
class QLabel;
class editor;

//mainwindow
class wgtTextView : public QMainWindow {
    Q_OBJECT

public:
    explicit wgtTextView(QWidget *parent = 0);
    bool loadFile(const QString &, char mode = 'r');
    void viewFile(const QString &);
    void editFile(const QString &);
    void writeSettings();
    void readSettings();
    ~wgtTextView();

protected:
    void closeEvent(QCloseEvent *);

private:
    bool             m_rwMode;

    QMenu           *m_menu;
    QMenu           *m_editMenu;

    QAction         *m_saveAct;
    QAction         *m_openAct;
    QAction         *m_fileSaveAsAct;
    QAction         *m_previewAct;
    QAction         *m_printAct;
    QAction         *m_quitAct;
    QAction         *m_findAtTextAct;
    QAction         *m_undoAct;
    QAction         *m_redoAct;
    QAction         *m_fontAct;

    enum { maxRecentFiles = 5 };
    QAction *m_recentFileAction[maxRecentFiles];

    void createMenu();
    void createActions();

    editor  *m_textView;
    QLabel  *m_docInfoLbl;
    QLabel  *m_fileInfoLbl;

    QPrinter printer;
    QSettings m_settings;

    QString pathToFile;
    QStringList m_lastOpenFiles;
    FindReplaceText *m_findReplace;
    void replace(const QString &, const QString &, QTextDocument::FindFlags);
    void replaceAll(const QString &, const QString &, QTextDocument::FindFlags);

private slots:
    bool openFile();
    bool saveAs();
    bool saveFile();
    bool printDoc();
    void previewDialog();
    void paintPreview(QPrinter *);
    void changeFont();
    void openRecentFile();


    //slots for wgtfindreplacetext
    void find();
    void findSlot(const QString &, QTextDocument::FindFlags);
    void replaceSlot(const QString &, QTextDocument::FindFlags, const QString &);
    void replaceAllSlot(const QString &, QTextDocument::FindFlags, const QString &);

};

//widget
class editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    editor(QWidget *parent = 0);
    void lineNumberPaint(QPaintEvent *);
    int linesNumberWidth();

private slots:
    void updateLinesWidth(int);
    void updateLineNumberArea(const QRect &, int);
    void docInfo();

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void info(QString);

private:
    QWidget *m_editorPaintArea;
};

//paint
class editorPaintArea : public QWidget {

public:
    editorPaintArea(editor *data) : QWidget(data) {
        m_editor = data;
    }

    QSize sizeHint() const {
        return QSize(m_editor->linesNumberWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        m_editor->lineNumberPaint(event);
    }

private:
    editor *m_editor;
};

#endif // WGTTEXTVIEW_H
