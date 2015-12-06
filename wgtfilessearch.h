#ifndef WGTFILESSEARCH_H
#define WGTFILESSEARCH_H

#include <QDialog>
#include <QMutex>
#include <QWaitCondition>

class QTabWidget;
class QLabel;
class QHBoxLayout;
class QLineEdit;
class QGridLayout;
class QCheckBox;
class QPushButton;
class QVBoxLayout;
class QListWidget;
class QString;
class QThread;
class filesSearchEngine;
class QListView;
class QStringListModel;

class wgtFilesSearch : public QDialog
{
    Q_OBJECT

public:
    explicit wgtFilesSearch(const QString &path, QWidget *parent = 0);
    ~wgtFilesSearch();

private slots:
    void withTextOptionSlot(bool);
    void startSearchFiles();
    void stopSearchFiles();
    void listOfFilesClicked();
    void addItemToStrList(QString);
    void resultToList();
    void enableBtn();

private:
    //GUI start
    QLabel *m_searchFileLbl;
    QLabel *m_pathToFileLbl;
    QLabel *m_withTextLbl;
    QLabel *m_searchPathLbl;

    QLineEdit *m_searchFileEdit;
    QLineEdit *m_pathToFileEdit;
    QLineEdit *m_withTextEdit;

    QCheckBox *m_withTextOption;
    QCheckBox *m_caseSensOption;
    QCheckBox *m_wholeWordsOption;

    QTabWidget *m_tabWgt;
    QListWidget *m_foundFileList;

    //tab pages
    QWidget *m_generalTab;
    QWidget *m_optionsTab;

    //layouts
    QGridLayout *m_upGridLayout;
    QHBoxLayout *m_upTopLayout;
    QVBoxLayout *m_rightBtnLayout;
    QVBoxLayout *m_mainBoxLayout;
    QHBoxLayout *m_bottomBtnLayout;

    //buttons
    QPushButton *m_btnSearch;
    QPushButton *m_btnStop;
    QPushButton *m_btnCancel;
    QPushButton *m_btnViewFile;
    QPushButton *m_btnGotoFile;
    QPushButton *m_btnNewSearch;
    //GUI end

    QString m_dirPath;
    QThread *m_fileSearchThread;
    filesSearchEngine *m_searchEngine;
    QStringList m_listOfFounfFiles;
    QStringListModel *model;
};

#endif // WGTFILESSEARCH_H

class filesSearchEngine : public QObject
{
    Q_OBJECT

public:
    explicit filesSearchEngine(QWidget *parent = 0);
    void loadSearchData(const QString&, const QString&);
    bool m_threadStop;
    void setStop() { m_threadStop = true; }

    //bool thread_Pause;
    // bool thread_Break;
    //bool Stop();
    //QMutex sync;
    //QWaitCondition pauseCond;

public slots:
    void process();

signals:
    void currentSearchPatch(QString);
    void foundFilePatch(QString);
    void finished();

private:
    QString m_strFileNames;
    QString m_dirPath;
};
