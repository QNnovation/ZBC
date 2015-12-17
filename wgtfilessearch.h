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
class QCalendarWidget;
class QComboBox;

class wgtFilesSearch : public QDialog
{
    Q_OBJECT

public:
    explicit wgtFilesSearch(const QString &path, QWidget *parent = 0);
    ~wgtFilesSearch();

private slots:
    void startSearchFiles();
    void stopSearchFiles();
    void listOfFilesClicked();
    void addItemToStrList(QString);
    void resultToList();
    void enableBtn();
    void setOptions();
    bool viewFile();
    void newSearch();

signals:
    void foundFileClicked(QString);

private:
    //GUI start
    QLabel *m_pSearchFileLbl;
    QLabel *m_pPathToFileLbl;
    QLabel *m_pWithTextLbl;
    QLabel *m_pSearchPathLbl;
    QLabel *m_pOptionsDateLbl;
    QLabel *m_pOptionsFromLbl;
    QLabel *m_pOptionsToLbl;
    QLabel *m_pOptionsSizeLbl;
    QLabel *m_pOptionsNote;

    //combo box
    QComboBox *m_pOptionsEqual;
    QComboBox *m_pOptionsParams;

    QLineEdit *m_pSearchFileEdit;
    QLineEdit *m_pPathToFileEdit;
    QLineEdit *m_pWithTextEdit;
    QLineEdit *m_pOptionsSizeLEdit;

    QCheckBox *m_pWithTextOption;
    QCheckBox *m_pOptionsDateCBox;
    QCheckBox *m_pOptionsSizeCBox;

    QTabWidget *m_pTabWgt;
    QListWidget *m_pFoundFileList;

    //tab pages
    QWidget *m_pGeneralTab;
    QWidget *m_pOptionsTab;
    QWidget *m_pTopWidget;

    //calendars for options tab
    QCalendarWidget *m_pOptionsCalendar_1;
    QCalendarWidget *m_pOptionsCalendar_2;

    //layouts
    QGridLayout *m_pUpGridLayout;
    QHBoxLayout *m_pOptionsHLayout_1;
    QHBoxLayout *m_pOptionsHLayout_2;
    QVBoxLayout *m_pOptionsVBox;
    QHBoxLayout *m_pUpTopLayout;
    QVBoxLayout *m_pRightBtnLayout;
    QVBoxLayout *m_pMainBoxLayout;
    QHBoxLayout *m_pBottomBtnLayout;

    //buttons
    QPushButton *m_pBtnSearch;
    QPushButton *m_pBtnStop;
    QPushButton *m_pBtnCancel;
    QPushButton *m_pBtnViewFile;
    QPushButton *m_pBtnGotoFile;
    QPushButton *m_pBtnNewSearch;
    //GUI end

    QString m_dirPath;
    QThread *m_pFileSearchThread;
    filesSearchEngine *m_pSearchEngine;

    //options tab variables
    bool m_optionsSizeBool;
    bool m_optionsDateBool;
    bool m_optionsWithTextBool;
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
    ~filesSearchEngine();

public slots:
    void process();

signals:
    void currentSearchPatch(QString);
    void foundFilePatch(QString);
    void finished();

private:
    QString m_strFileNames;
    QString m_dirPath;
    QStringList m_strListNames;
};
