#include "wgtfilessearch.h"
#include "wgttextview.h"
#include "zbc_styles.h"

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
#include <QMessageBox>
#include <QListWidgetItem>
#include <QApplication>

wgtFilesSearch::wgtFilesSearch(const QString &path, QWidget *parent) : QDialog(parent)
{
    this->resize(650, 170);
    this->setWindowTitle("File search");
    this->setModal(true);
    this->setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

    QApplication::setStyle(new ZBC_SimpleStyle);

    if (path.isEmpty())
        return;
    else
        m_dirPath = path;

    m_optionsSizeBool = false;
    m_optionsDateBool = false;
    m_optionsWithTextBool = false;

    m_pSearchFileLbl = new QLabel(tr("Files:"), this);
    m_pPathToFileLbl = new QLabel(tr("Path:"), this);
    m_pSearchFileEdit = new QLineEdit(this);
    m_pPathToFileEdit = new QLineEdit(this);
    m_pPathToFileEdit->setText(m_dirPath);

    m_pWithTextLbl = new QLabel(tr("With text:"), this);
    m_pWithTextEdit = new QLineEdit(this);

    m_pWithTextOption = new QCheckBox(this);

    //general tab
    m_pUpGridLayout = new QGridLayout(this);
    m_pUpGridLayout->addWidget(m_pSearchFileLbl, 0, 0);
    m_pUpGridLayout->addWidget(m_pSearchFileEdit, 0, 1);
    m_pUpGridLayout->addWidget(m_pPathToFileLbl, 1, 0);
    m_pUpGridLayout->addWidget(m_pPathToFileEdit, 1, 1);

    QFrame *lineOnWgt = new QFrame(this);
    lineOnWgt->setFrameStyle(QFrame::HLine | QFrame::Raised);
    m_pUpGridLayout->addWidget(lineOnWgt, 2, 0, 1, 4);

    m_pUpGridLayout->addWidget(m_pWithTextLbl, 3, 0);
    m_pUpGridLayout->addWidget(m_pWithTextEdit, 3, 1);
    m_pUpGridLayout->addWidget(m_pWithTextOption, 3, 2);

    //options tab
    //data options
    m_pOptionsDateLbl = new QLabel(tr("Date: "), this);
    m_pOptionsDateCBox = new QCheckBox(this);
    m_pOptionsFromLbl = new QLabel(tr("From: "));
    m_pOptionsCalendar_1 = new QCalendarWidget(this);
    m_pOptionsCalendar_1->setEnabled(false);
    m_pOptionsToLbl = new QLabel(tr("To: "));
    m_pOptionsCalendar_2 = new QCalendarWidget(this);
    m_pOptionsCalendar_2->setEnabled(false);

    m_pOptionsHLayout_1 = new QHBoxLayout();
    m_pOptionsHLayout_1->addWidget(m_pOptionsDateLbl);
    m_pOptionsHLayout_1->addWidget(m_pOptionsDateCBox);
    m_pOptionsHLayout_1->addWidget(m_pOptionsFromLbl);
    m_pOptionsHLayout_1->addWidget(m_pOptionsCalendar_1);
    m_pOptionsHLayout_1->addWidget(m_pOptionsToLbl);
    m_pOptionsHLayout_1->addWidget(m_pOptionsCalendar_2);
    //size options
    m_pOptionsSizeLbl = new QLabel(tr("Size: "), this);
    m_pOptionsSizeCBox = new QCheckBox(this);
    m_pOptionsEqual = new QComboBox(this);
    m_pOptionsEqual->setEnabled(false);
    QStringList optionsEqual;
    optionsEqual << "=" << "<" << ">";
    m_pOptionsEqual->addItems(optionsEqual);
    m_pOptionsSizeLEdit = new QLineEdit(this);
    QRegExp rx("[0-9]\\d{0,75}");
    QValidator *validator = new QRegExpValidator(rx, this);
    m_pOptionsSizeLEdit->setValidator(validator);
    m_pOptionsSizeLEdit->setEnabled(false);
    m_pOptionsParams = new QComboBox(this);
    QStringList optionsParams;
    optionsParams << "By" << "Kb" << "Mb" << "Gb";
    m_pOptionsParams->addItems(optionsParams);
    m_pOptionsParams->setEnabled(false);

    m_pOptionsHLayout_2 = new QHBoxLayout();
    m_pOptionsHLayout_2->addWidget(m_pOptionsSizeLbl);
    m_pOptionsHLayout_2->addWidget(m_pOptionsSizeCBox);
    m_pOptionsHLayout_2->addWidget(m_pOptionsEqual);
    m_pOptionsHLayout_2->addWidget(m_pOptionsSizeLEdit);
    m_pOptionsHLayout_2->addWidget(m_pOptionsParams);

    m_pOptionsNote = new QLabel(tr("[ = work's only with bytes ]"), this);

    m_pOptionsVBox = new QVBoxLayout();
    m_pOptionsVBox->addLayout(m_pOptionsHLayout_1);
    m_pOptionsVBox->addLayout(m_pOptionsHLayout_2);
    m_pOptionsVBox->addWidget(m_pOptionsNote);
    //tab pages
    m_pGeneralTab = new QWidget(this);
    m_pGeneralTab->setLayout(m_pUpGridLayout);
    m_pOptionsTab = new QWidget(this);
    m_pOptionsTab->setLayout(m_pOptionsVBox);

    //tab
    m_pTopWidget = new QWidget(this);
    m_pTopWidget->setFixedHeight(250);

    m_pTabWgt = new QTabWidget(this);
    m_pTabWgt->addTab(m_pGeneralTab, QString(tr("General")));
    m_pTabWgt->addTab(m_pOptionsTab, QString(tr("Options")));

    m_pUpTopLayout = new QHBoxLayout(m_pTopWidget);
    m_pUpTopLayout->addWidget(m_pTabWgt);

    m_pRightBtnLayout = new QVBoxLayout();
    m_pBtnSearch = new QPushButton(tr("Search"), this);
    m_pBtnStop = new QPushButton(tr("Stop"), this);
    m_pBtnStop->setEnabled(false);
    m_pBtnCancel = new QPushButton(tr("Cancel"), this);
    m_pRightBtnLayout->addSpacing(20);
    m_pRightBtnLayout->addWidget(m_pBtnSearch);
    m_pRightBtnLayout->addWidget(m_pBtnStop);
    m_pRightBtnLayout->addWidget(m_pBtnCancel);
    m_pRightBtnLayout->addStretch();
    m_pUpTopLayout->addLayout(m_pRightBtnLayout);

    m_pMainBoxLayout = new QVBoxLayout(this);
    m_pFoundFileList = new QListWidget(this);
    m_pFoundFileList->setUniformItemSizes(true);

    m_pMainBoxLayout->addWidget(m_pTopWidget);
    m_pSearchPathLbl = new QLabel(tr(":"), this);

    m_pMainBoxLayout->addWidget(m_pFoundFileList);

    m_pBottomBtnLayout = new QHBoxLayout();
    m_pBtnViewFile = new QPushButton(tr("View"), this);
    m_pBtnGotoFile = new QPushButton(tr("Go to file"), this);
    m_pBtnNewSearch = new QPushButton(tr("New Search"), this);
    m_pBottomBtnLayout->addWidget(m_pBtnViewFile);
    m_pBottomBtnLayout->addWidget(m_pBtnGotoFile);
    m_pBottomBtnLayout->addWidget(m_pBtnNewSearch);

    m_pBtnNewSearch->setEnabled(false);
    m_pBtnViewFile->setEnabled(false);
    m_pBtnGotoFile->setEnabled(false);

    m_pMainBoxLayout->addLayout(m_pBottomBtnLayout);
    m_pMainBoxLayout->addWidget(m_pSearchPathLbl);

    setLayout(m_pMainBoxLayout);

    //states on form create
    m_pWithTextEdit->setEnabled(false);

    //connect
    connect(m_pBtnCancel, &QPushButton::clicked, this, &QDialog::close);
    connect(m_pBtnSearch, &QPushButton::pressed, this, &wgtFilesSearch::startSearchFiles);
    connect(m_pBtnStop, &QPushButton::pressed, this, &wgtFilesSearch::stopSearchFiles);
    connect(m_pFoundFileList, &QListWidget::itemDoubleClicked, this, &wgtFilesSearch::listOfFilesClicked);
    connect(m_pOptionsDateCBox, &QCheckBox::released, this, &wgtFilesSearch::setOptions);
    connect(m_pOptionsSizeCBox, &QCheckBox::released, this, &wgtFilesSearch::setOptions);
    connect(m_pWithTextOption, QCheckBox::released, this, &wgtFilesSearch::setOptions);
    connect(m_pBtnViewFile, &QPushButton::clicked, this, &wgtFilesSearch::viewFile);
    connect(m_pBtnNewSearch, &QPushButton::clicked, this, &wgtFilesSearch::newSearch);
}

void wgtFilesSearch::startSearchFiles()
{
    m_pBtnStop->setEnabled(true);

    m_pBtnNewSearch->setEnabled(false);
    m_pBtnViewFile->setEnabled(false);
    m_pBtnGotoFile->setEnabled(false);

    m_pFoundFileList->clear();
    //thread
    m_pFileSearchThread = new QThread();
    m_pSearchEngine = new filesSearchEngine();           //<<search class
    m_pSearchEngine->moveToThread(m_pFileSearchThread);

    //start class in thread
    connect(m_pFileSearchThread, &QThread::started,
            m_pSearchEngine, &filesSearchEngine::process);

    //close thread
    connect(m_pSearchEngine, &filesSearchEngine::finished,
            m_pFileSearchThread, QThread::quit);

    //delete class after work
    connect(m_pSearchEngine, &filesSearchEngine::finished,
            m_pSearchEngine, &filesSearchEngine::deleteLater);

    //delete thread after work
    connect(m_pFileSearchThread, &QThread::finished,
            m_pFileSearchThread, &QThread::deleteLater);

    //data from thread
    connect(m_pSearchEngine, &filesSearchEngine::currentSearchPatch,
            m_pSearchPathLbl, &QLabel::setText);
    connect(m_pSearchEngine, &filesSearchEngine::foundFilePatch,
            this, &wgtFilesSearch::addItemToStrList);
    connect(m_pFileSearchThread, &QThread::finished,
            this, &wgtFilesSearch::enableBtn);

    m_pSearchEngine->loadSearchData(m_pSearchFileEdit->text(), m_pPathToFileEdit->text());
    m_pFileSearchThread->start();
    m_pBtnSearch->setEnabled(false);
    m_pSearchPathLbl->resize(width(), 10);
}

void wgtFilesSearch::stopSearchFiles()
{
    m_pSearchEngine->setStop();
    m_pBtnSearch->setEnabled(true);
    m_pBtnStop->setEnabled(false);
}

void wgtFilesSearch::addItemToStrList(QString data)
{
    m_pFoundFileList->addItem(data);
}

void wgtFilesSearch::resultToList()
{
    QStringList filesPaths, tmpSList;
    for (int i = 0; i < m_pFoundFileList->count(); ++i) {
        filesPaths.append(m_pFoundFileList->item(i)->text());
    }
    //file date option is enable
    if (m_optionsDateBool) {
        for (int m = 0; m < filesPaths.size(); ++m) {
            QDateTime dateTime_1(m_pOptionsCalendar_1->selectedDate());
            QDateTime dateTime_2(m_pOptionsCalendar_2->selectedDate());
            if ((QFileInfo(filesPaths.at(m)).created() > dateTime_1) &&
                    (QFileInfo(filesPaths.at(m)).created() < dateTime_2))
                tmpSList.append(filesPaths.at(m));
        }
        filesPaths.clear();
        filesPaths.append(tmpSList);
        tmpSList.clear();

    }
    //file size options is enable
    double optionSizeOfOFile = m_pOptionsSizeLEdit->text().toDouble();
    QChar optionsSign = m_pOptionsParams->currentText().at(0);
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
            if (m_pOptionsEqual->currentText() == "=") {
                if (QFileInfo(filesPaths.at(k)).size() == (optionSizeOfOFile * bytesToKlMbGb))
                    tmpSList.append(filesPaths.at(k));
            }
            else if (m_pOptionsEqual->currentText() == "<") {
                if (QFileInfo(filesPaths.at(k)).size() < (optionSizeOfOFile * bytesToKlMbGb))
                    tmpSList.append(filesPaths.at(k));
            }
            else if (m_pOptionsEqual->currentText() == ">") {
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
                        if (buf.contains(m_pWithTextEdit->text()))
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
    m_pFoundFileList->clear();
    m_pFoundFileList->addItem(total);
    for (int k = 0; k < filesPaths.size(); ++k) {
        m_pFoundFileList->addItem(filesPaths.at(k));
    }
    m_pFoundFileList->setCurrentRow(0);
}

void wgtFilesSearch::enableBtn()
{
    m_pBtnSearch->setEnabled(true);
    m_pBtnViewFile->setEnabled(true);
    m_pBtnGotoFile->setEnabled(true);
    m_pBtnNewSearch->setEnabled(true);
    m_pBtnStop->setEnabled(false);
    resultToList();
}

wgtFilesSearch::~wgtFilesSearch()
{

}

void wgtFilesSearch::setOptions()
{
    //options tab
    //date of file
    if (m_pOptionsDateCBox->isChecked())
    {
        m_pOptionsCalendar_1->setEnabled(true);
        m_pOptionsCalendar_2->setEnabled(true);
        m_optionsDateBool = true;
    }
    else {
        m_pOptionsCalendar_1->setEnabled(false);
        m_pOptionsCalendar_2->setEnabled(false);
        m_optionsDateBool = false;
    }
    //size of file
    if (m_pOptionsSizeCBox->isChecked()) {
        m_pOptionsEqual->setEnabled(true);
        m_pOptionsSizeLEdit->setEnabled(true);
        m_pOptionsParams->setEnabled(true);
        m_optionsSizeBool = true;
    }
    else {
        m_pOptionsEqual->setEnabled(false);
        m_pOptionsSizeLEdit->setEnabled(false);
        m_pOptionsParams->setEnabled(false);
        m_optionsSizeBool = false;
    }
    //main tab
    if (m_pWithTextOption->isChecked()) {
        m_optionsWithTextBool = true;
        m_pWithTextEdit->setEnabled(true);
    }
    else {
        m_optionsWithTextBool = false;
        m_pWithTextEdit->setEnabled(false);
    }
}

bool wgtFilesSearch::viewFile()
{
    QString pathToFile(m_pFoundFileList->currentItem()->text());
    if (QFileInfo(pathToFile).isFile()) {
        wgtTextView *textViewer = new wgtTextView(this);
        textViewer->loadFile(pathToFile);
        textViewer->show();
        return true;
    }
    else
        return false;
}

void wgtFilesSearch::newSearch()
{
    m_pFoundFileList->clear();
    m_pFoundFileList->addItem(tr("[ Ready ]"));
    m_pFoundFileList->setCurrentRow(0);
}

void wgtFilesSearch::listOfFilesClicked()
{
    emit foundFileClicked(m_pFoundFileList->currentItem()->text());
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
        int m = 0;
        for(int i = 0;  i < m_strFileNames.size(); ++i) {
            if (m_strFileNames.at(i) != ' ') {
                tmp.append(m_strFileNames.at(i));
                ++m;
            }
            else {
                if (m >= 1) {
                    m_strListNames.append(tmp);
                    tmp.clear();
                    m = 0;
                }
            }
        }
    }
    else
        m_strListNames.append(m_strFileNames);
}

void filesSearchEngine::process()
{
    for (int i = 0; i < m_strListNames.size(); ++i) {
        m_strFileNames.clear();
        m_strFileNames = m_strListNames.at(i);

        QStringList nameFilter;
        int starCnt = 0;
        if (!m_strFileNames.isEmpty()) {
            if (m_strFileNames.contains("*")) {
                //if have stars,count it
                for (int i = 0; i < m_strFileNames.size(); ++i)
                    if (m_strFileNames.at(i) == '*')
                        ++starCnt;
                //if number of stars = 1
                if (starCnt == 1 && m_strFileNames.at(0) == '*') {
                    nameFilter.append(m_strFileNames);
                    m_strFileNames.clear();
                }
                //if number of stars > 1;
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
}


filesSearchEngine::~filesSearchEngine()
{

}











