#include "wgtfilessearch.h"

#include <QTabWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QGridLayout>
#include <QFrame>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QDirIterator>
#include <QThread>
#include <QFileInfo>

wgtFilesSearch::wgtFilesSearch(const QString &path, QWidget *parent) : QDialog(parent)
{
    this->resize(450, 170);
    this->setWindowTitle("File search");
    this->setModal(true);

    if (path.isEmpty())
        return;
    else
        m_dirPath = path;

    m_searchFileLbl = new QLabel(tr("Files:"), this);
    m_pathToFileLbl = new QLabel(tr("Path:"), this);
    m_searchFileEdit = new QLineEdit(this);
    m_pathToFileEdit = new QLineEdit(this);
    m_pathToFileEdit->setText(m_dirPath);

    m_withTextLbl = new QLabel(tr("With text:"), this);
    m_withTextEdit = new QLineEdit(this);

    m_withTextOption = new QCheckBox(this);
    m_caseSensOption = new QCheckBox(tr("Case sensitive"), this);
    m_wholeWordsOption = new QCheckBox(tr("Only whole words"), this);

    m_upGridLayout = new QGridLayout(this);
    m_upGridLayout->addWidget(m_searchFileLbl, 0, 0);
    m_upGridLayout->addWidget(m_searchFileEdit, 0, 1);
    m_upGridLayout->addWidget(m_pathToFileLbl, 1, 0);
    m_upGridLayout->addWidget(m_pathToFileEdit, 1, 1);

    QFrame *lineOnWgt = new QFrame(this);
    lineOnWgt->setFrameStyle(QFrame::HLine | QFrame::Raised);
    m_upGridLayout->addWidget(lineOnWgt, 2, 0, 1, 4);

    m_upGridLayout->addWidget(m_withTextLbl, 3, 0);
    m_upGridLayout->addWidget(m_withTextEdit, 3, 1);
    m_upGridLayout->addWidget(m_withTextOption, 3, 2);
    m_upGridLayout->addWidget(m_caseSensOption, 4, 0);
    m_upGridLayout->addWidget(m_wholeWordsOption, 5, 0);

    //tab pages
    m_generalTab = new QWidget(this);
    m_generalTab->setLayout(m_upGridLayout);

    //tab
    m_tabWgt = new QTabWidget(this);
    m_tabWgt->addTab(m_generalTab, QString(tr("General")));
    m_tabWgt->addTab(new QLabel(tr("2")), QString(tr("Options")));

    m_upTopLayout = new QHBoxLayout();
    m_upTopLayout->addWidget(m_tabWgt);

    m_rightBtnLayout = new QVBoxLayout();
    m_btnSearch = new QPushButton(tr("Search"), this);
    m_btnStop = new QPushButton(tr("Stop"), this);
    m_btnStop->setEnabled(false);
    m_btnCancel = new QPushButton(tr("Cancel"), this);
    m_rightBtnLayout->addSpacing(20);
    m_rightBtnLayout->addWidget(m_btnSearch);
    m_rightBtnLayout->addWidget(m_btnStop);
    m_rightBtnLayout->addWidget(m_btnCancel);
    m_rightBtnLayout->addStretch();
    m_upTopLayout->addLayout(m_rightBtnLayout);

    m_mainBoxLayout = new QVBoxLayout(this);
    m_foundFileList = new QListWidget(this);
    //listWgt->setVisible(false);
    m_mainBoxLayout->addLayout(m_upTopLayout);
    m_searchPathLbl = new QLabel(tr(":"), this);

    m_mainBoxLayout->addWidget(m_searchPathLbl);
    m_mainBoxLayout->addWidget(m_foundFileList);

    m_bottomBtnLayout = new QHBoxLayout();
    m_btnViewFile = new QPushButton(tr("View"), this);
    m_btnGotoFile = new QPushButton(tr("Go to file"), this);
    m_btnNewSearch = new QPushButton(tr("New Search"), this);
    m_bottomBtnLayout->addWidget(m_btnViewFile);
    m_bottomBtnLayout->addWidget(m_btnGotoFile);
    m_bottomBtnLayout->addWidget(m_btnNewSearch);

    m_mainBoxLayout->addLayout(m_bottomBtnLayout);

    setLayout(m_mainBoxLayout);

    //states on form create
    m_withTextEdit->setEnabled(false);
    m_caseSensOption->setEnabled(false);
    m_wholeWordsOption->setEnabled(false);

    //connect
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::close);
    connect(m_withTextOption, &QCheckBox::toggled, this, &wgtFilesSearch::withTextOptionSlot);
    connect(m_btnSearch, &QPushButton::pressed, this, &wgtFilesSearch::startSearchFiles);
    connect(m_btnStop, QPushButton::pressed, this, &wgtFilesSearch::stopSearchFiles);
    connect (m_foundFileList, &QListWidget::itemDoubleClicked, this, &wgtFilesSearch::listOfFilesClicked);

    //thread
    m_fileSearchThread = new QThread();
    m_searchEngine = new filesSearchEngine();           //<<search class
    m_searchEngine->moveToThread(m_fileSearchThread);

    //start class in thread
    connect(m_fileSearchThread, &QThread::started,
            m_searchEngine, &filesSearchEngine::process);
    //close thread
    connect(m_searchEngine, &filesSearchEngine::finished,
            m_fileSearchThread, QThread::quit);
    //delete class after work
    connect(m_searchEngine, &filesSearchEngine::finished,
            m_searchEngine, &filesSearchEngine::deleteLater);
    //delete thread after work
    connect(m_fileSearchThread, &QThread::finished,
            m_fileSearchThread, &QThread::deleteLater);

    //data from thread
    connect(m_searchEngine, &filesSearchEngine::currentSearchPatch,
            m_searchPathLbl, &QLabel::setText);
    connect(m_searchEngine, &filesSearchEngine::foundFilePatch,
            this, &wgtFilesSearch::addItemToFileList);
}

void wgtFilesSearch::startSearchFiles()
{
    m_searchEngine->loadSearchData(m_searchFileEdit->text(), m_pathToFileEdit->text());
    m_fileSearchThread->start();
    m_btnSearch->setEnabled(false);
    m_btnStop->setEnabled(true);
}

void wgtFilesSearch::stopSearchFiles()
{
    m_fileSearchThread->wait();
    m_fileSearchThread->quit();
    m_btnSearch->setEnabled(true);
}

void wgtFilesSearch::resultToList()
{
    QStringList fdPath;
    for(int i = 0; i < m_foundFileList->count(); ++i)
    {
        fdPath.append(m_foundFileList->item(i)->text());
    }
    qDebug() << "Size: " << fdPath.size();
}

void wgtFilesSearch::withTextOptionSlot(bool state)
{
    m_withTextEdit->setEnabled(state);
    m_caseSensOption->setEnabled(state);
    m_wholeWordsOption->setEnabled(state);
}

void wgtFilesSearch::listOfFilesClicked()
{
    emit(m_foundFileList->currentItem()->text());
}

void wgtFilesSearch::addItemToFileList(QString data)
{
    m_foundFileList->addItem(data);
}

//filesearch engine implementation
filesSearchEngine::filesSearchEngine(QWidget *parent) : QObject(parent)
{

}

void filesSearchEngine::loadSearchData(const QString &files, const QString &path)
{
    m_strFileNames = files;
    m_dirPath = path;
}

void filesSearchEngine::process()
{
    QDirIterator it(m_dirPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        emit currentSearchPatch(it.filePath());
        if (it.fileName() == m_strFileNames) {
            emit foundFilePatch(it.filePath());
        }
        else if (m_strFileNames.isEmpty()) {
            emit foundFilePatch(it.filePath());
        }
    }
    emit currentSearchPatch(":");
    emit finished();
}












