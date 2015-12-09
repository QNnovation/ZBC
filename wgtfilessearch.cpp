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
#include <QCalendarWidget>
#include <QComboBox>

wgtFilesSearch::wgtFilesSearch(const QString &path, QWidget *parent) : QDialog(parent)
{
    this->resize(650, 170);
    this->setWindowTitle("File search");
    this->setModal(true);
    this->setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

    if (path.isEmpty())
        return;
    else
        m_dirPath = path;

    m_optionsSizeBool = false;
    m_optionsDateBool = false;
    m_optionsWithTextBool = false;

    m_searchFileLbl = new QLabel(tr("Files:"), this);
    m_pathToFileLbl = new QLabel(tr("Path:"), this);
    m_searchFileEdit = new QLineEdit(this);
    m_pathToFileEdit = new QLineEdit(this);
    m_pathToFileEdit->setText(m_dirPath);

    m_withTextLbl = new QLabel(tr("With text:"), this);
    m_withTextEdit = new QLineEdit(this);

    m_withTextOption = new QCheckBox(this);

    //general tab
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

    //options tab
    //data options
    m_optionsDateLbl = new QLabel(tr("Date: "), this);
    m_optionsDateCBox = new QCheckBox(this);
    m_optionsFromLbl = new QLabel(tr("From: "));
    m_optionsCalendar_1 = new QCalendarWidget(this);
    m_optionsCalendar_1->setEnabled(false);
    m_optionsToLbl = new QLabel(tr("To: "));
    m_optionsCalendar_2 = new QCalendarWidget(this);
    m_optionsCalendar_2->setEnabled(false);

    m_optionsHLayout_1 = new QHBoxLayout();
    m_optionsHLayout_1->addWidget(m_optionsDateLbl);
    m_optionsHLayout_1->addWidget(m_optionsDateCBox);
    m_optionsHLayout_1->addWidget(m_optionsFromLbl);
    m_optionsHLayout_1->addWidget(m_optionsCalendar_1);
    m_optionsHLayout_1->addWidget(m_optionsToLbl);
    m_optionsHLayout_1->addWidget(m_optionsCalendar_2);
    //size options
    m_optionsSizeLbl = new QLabel(tr("Size: "), this);
    m_optionsSizeCBox = new QCheckBox(this);
    m_optionsEqual = new QComboBox(this);
    m_optionsEqual->setEnabled(false);
    QStringList optionsEqual;
    optionsEqual << "=" << "<" << ">";
    m_optionsEqual->addItems(optionsEqual);
    m_optionsSizeLEdit = new QLineEdit(this);
    m_optionsSizeLEdit->setEnabled(false);
    m_optionsParams = new QComboBox(this);
    QStringList optionsParams;
    optionsParams << "By" << "Kb" << "Mb" << "Gb";
    m_optionsParams->addItems(optionsParams);
    m_optionsParams->setEnabled(false);

    m_optionsHLayout_2 = new QHBoxLayout();
    m_optionsHLayout_2->addWidget(m_optionsSizeLbl);
    m_optionsHLayout_2->addWidget(m_optionsSizeCBox);
    m_optionsHLayout_2->addWidget(m_optionsEqual);
    m_optionsHLayout_2->addWidget(m_optionsSizeLEdit);
    m_optionsHLayout_2->addWidget(m_optionsParams);

    m_optionsNote = new QLabel(tr("[ = work's only with bytes ]"), this);

    m_optionsVBox = new QVBoxLayout();
    m_optionsVBox->addLayout(m_optionsHLayout_1);
    m_optionsVBox->addLayout(m_optionsHLayout_2);
    m_optionsVBox->addWidget(m_optionsNote);
    //tab pages
    m_generalTab = new QWidget(this);
    m_generalTab->setLayout(m_upGridLayout);
    m_optionsTab = new QWidget(this);
    m_optionsTab->setLayout(m_optionsVBox);

    //tab
    m_topWidget = new QWidget(this);
    m_topWidget->setFixedHeight(250);

    m_tabWgt = new QTabWidget(this);
    m_tabWgt->addTab(m_generalTab, QString(tr("General")));
    m_tabWgt->addTab(m_optionsTab, QString(tr("Options")));

    m_upTopLayout = new QHBoxLayout(m_topWidget);
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
    m_foundFileList->setUniformItemSizes(true);

    m_mainBoxLayout->addWidget(m_topWidget);
    m_searchPathLbl = new QLabel(tr(":"), this);

    m_mainBoxLayout->addWidget(m_foundFileList);

    m_bottomBtnLayout = new QHBoxLayout();
    m_btnViewFile = new QPushButton(tr("View"), this);
    m_btnGotoFile = new QPushButton(tr("Go to file"), this);
    m_btnNewSearch = new QPushButton(tr("New Search"), this);
    m_bottomBtnLayout->addWidget(m_btnViewFile);
    m_bottomBtnLayout->addWidget(m_btnGotoFile);
    m_bottomBtnLayout->addWidget(m_btnNewSearch);

    m_mainBoxLayout->addLayout(m_bottomBtnLayout);
    m_mainBoxLayout->addWidget(m_searchPathLbl);

    setLayout(m_mainBoxLayout);

    //states on form create
    m_withTextEdit->setEnabled(false);

    //connect
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::close);
    connect(m_btnSearch, &QPushButton::pressed, this, &wgtFilesSearch::startSearchFiles);
    connect(m_btnStop, &QPushButton::pressed, this, &wgtFilesSearch::stopSearchFiles);
    connect(m_foundFileList, &QListWidget::itemDoubleClicked, this, &wgtFilesSearch::listOfFilesClicked);
    connect(m_optionsDateCBox, &QCheckBox::released, this, &wgtFilesSearch::setOptions);
    connect(m_optionsSizeCBox, &QCheckBox::released, this, &wgtFilesSearch::setOptions);
    connect(m_withTextOption, QCheckBox::released, this, &wgtFilesSearch::setOptions);
}

void wgtFilesSearch::startSearchFiles()
{
    m_btnStop->setEnabled(true);
    m_foundFileList->clear();
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
            this, &wgtFilesSearch::addItemToStrList);
    connect(m_fileSearchThread, &QThread::finished,
            this, &wgtFilesSearch::enableBtn);

    m_searchEngine->loadSearchData(m_searchFileEdit->text(), m_pathToFileEdit->text());
    m_fileSearchThread->start();
    m_btnSearch->setEnabled(false);
    m_btnStop->setEnabled(true);
    m_searchPathLbl->resize(width(), 10);
}

void wgtFilesSearch::stopSearchFiles()
{
    m_searchEngine->setStop();
    m_btnSearch->setEnabled(true);
    m_btnStop->setEnabled(false);
}

void wgtFilesSearch::addItemToStrList(QString data)
{
    m_foundFileList->addItem(data);
}

void wgtFilesSearch::resultToList()
{
    QStringList filesPaths, tmpSList;
    for (int i = 0; i < m_foundFileList->count(); ++i) {
        filesPaths.append(m_foundFileList->item(i)->text());
    }
    //file date option is enable
    if (m_optionsDateBool) {
        for (int m = 0; m < filesPaths.size(); ++m) {
            QDateTime dateTime_1(m_optionsCalendar_1->selectedDate());
            QDateTime dateTime_2(m_optionsCalendar_2->selectedDate());
            if ((QFileInfo(filesPaths.at(m)).created() > dateTime_1) &&
                    (QFileInfo(filesPaths.at(m)).created() < dateTime_2))
                tmpSList.append(filesPaths.at(m));
        }
        filesPaths.clear();
        filesPaths.append(tmpSList);
        tmpSList.clear();

    }
    //file size options is enable
    double optionSizeOfOFile = m_optionsSizeLEdit->text().toDouble();
    QChar optionsSign = m_optionsParams->currentText().at(0);
    int bytesToKlMbGb = 0;
    if (optionsSign == 'B')
        bytesToKlMbGb = 1;
    else if (optionsSign == 'K')
        bytesToKlMbGb = 1024;
    else if (optionsSign == 'M')
        bytesToKlMbGb = 1048576;
    else if (optionsSign == 'G')
        bytesToKlMbGb = 1073741824;

    if (m_optionsSizeBool) {
        for (int k = 0; k < filesPaths.size(); ++ k) {
            if (m_optionsEqual->currentText() == "=") {
                if (QFileInfo(filesPaths.at(k)).size() == (optionSizeOfOFile * bytesToKlMbGb))
                    tmpSList.append(filesPaths.at(k));
            }
            else if (m_optionsEqual->currentText() == "<") {
                if (QFileInfo(filesPaths.at(k)).size() < (optionSizeOfOFile * bytesToKlMbGb))
                    tmpSList.append(filesPaths.at(k));
            }
            else if (m_optionsEqual->currentText() == ">") {
                if (QFileInfo(filesPaths.at(k)).size() > (optionSizeOfOFile * bytesToKlMbGb))
                    tmpSList.append(filesPaths.at(k));
            }
        }
        filesPaths.clear();
        filesPaths.append(tmpSList);
        tmpSList.clear();
    }
    //search text in file enabled
    if (m_optionsWithTextBool) {
        for (int n = 0; n < filesPaths.size(); ++n) {
            if (QFileInfo(filesPaths.at(n)).isFile()) {
                QFile inputFile(filesPaths.at(n));
                if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
                    qDebug() << "Could not open the input file (FileSearch) \n "<< filesPaths.at(n);
                else {
                    while(!inputFile.atEnd()) {
                        QString buf = inputFile.readLine();
                        if (buf.contains(m_withTextEdit->text()))
                            tmpSList.append(filesPaths.at(n));
                    }
                    inputFile.close();
                }
            }
        }
        filesPaths.clear();
        filesPaths.append(tmpSList);
        tmpSList.clear();
    }

    int dry = 0, fls = 0;
    for (int j = 0; j < filesPaths.size(); ++j) {
        if (QFileInfo(filesPaths.at(j)).isDir())
            ++dry;
        else
            ++fls;
    }
    QString total = "[ Files: " + QString::number(fls) + " Directory: " + QString::number(dry) + " ]";
    m_foundFileList->clear();
    m_foundFileList->addItem(total);
    for (int k = 0; k < filesPaths.size(); ++k) {
        m_foundFileList->addItem(filesPaths.at(k));
    }
}

void wgtFilesSearch::enableBtn()
{
    m_btnSearch->setEnabled(true);
    resultToList();
}

wgtFilesSearch::~wgtFilesSearch()
{

}

void wgtFilesSearch::setOptions()
{
    //options tab
    //date of file
    if (m_optionsDateCBox->isChecked())
    {
        m_optionsCalendar_1->setEnabled(true);
        m_optionsCalendar_2->setEnabled(true);
        m_optionsDateBool = true;
    }
    else {
        m_optionsCalendar_1->setEnabled(false);
        m_optionsCalendar_2->setEnabled(false);
        m_optionsDateBool = false;
    }
    //size of file
    if (m_optionsSizeCBox->isChecked()) {
        m_optionsEqual->setEnabled(true);
        m_optionsSizeLEdit->setEnabled(true);
        m_optionsParams->setEnabled(true);
        m_optionsSizeBool = true;
    }
    else {
        m_optionsEqual->setEnabled(false);
        m_optionsSizeLEdit->setEnabled(false);
        m_optionsParams->setEnabled(false);
        m_optionsSizeBool = false;
    }
    //main tab
    if (m_withTextOption->isChecked()) {
        m_optionsWithTextBool = true;
        m_withTextEdit->setEnabled(true);
    }
    else {
        m_optionsWithTextBool = false;
        m_withTextEdit->setEnabled(false);
    }
}

void wgtFilesSearch::listOfFilesClicked()
{
    emit foundFileClicked(m_foundFileList->currentItem()->text());
}

//filesearch engine implementation
filesSearchEngine::filesSearchEngine(QWidget *parent) : QObject(parent)
{
    m_threadStop = false;
}

void filesSearchEngine::loadSearchData(const QString &files, const QString &path)
{
    m_strFileNames = files;
    m_dirPath = path;

    if (m_strFileNames.contains(" ")) {
        m_strFileNames.push_back(" ");
        QString tmp;
        for(int i = 0;  i < m_strFileNames.size(); ++i) {
            if (m_strFileNames.at(i) != ' ') {
                tmp.append(m_strFileNames.at(i));
            }
            else {
                m_strListNames.append(tmp);
                tmp.clear();
            }
        }
        for (int j = 0; j < m_strListNames.size(); ++j)
            if (m_strListNames.at(j).size() == 0)
                m_strListNames.removeAt(j);
    }
    qDebug() << m_strListNames;
}

void filesSearchEngine::process()
{
    QStringList nameFilter;
    int starCnt = 0;
    if (!m_strFileNames.isEmpty()) {
        if (m_strFileNames.contains("*")) {
            //если есть звёзды то считаем их
            for (int i = 0; i < m_strFileNames.size(); ++i)
                if (m_strFileNames.at(i) == '*')
                    ++starCnt;
            //если количество звёзд равно 1 то создаём фильтр
            if (starCnt == 1 && m_strFileNames.at(0) == '*') {
                nameFilter.append(m_strFileNames);
                m_strFileNames.clear();
            }
            //если количество звёзд > 1;
            else if (starCnt == 2 && m_strFileNames == "*.*") {
                m_strFileNames.clear();
            }
        }
    }
    QString _upper = m_strFileNames.toUpper();
    QString _lower = m_strFileNames.toLower();

    QDirIterator it(m_dirPath, nameFilter, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (m_threadStop)
            break;
        emit currentSearchPatch(it.filePath());
        if (it.fileName().contains(m_strFileNames)
                || it.fileName().contains(_upper)
                || it.fileName().contains(_lower))
        {
            emit foundFilePatch(it.filePath());
        }
        else if (m_strFileNames.isEmpty())
        {
            emit foundFilePatch(it.filePath());
        }
    }
    emit currentSearchPatch(":");
    emit finished();
}


filesSearchEngine::~filesSearchEngine()
{

}











