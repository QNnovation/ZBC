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

wgtFilesSearch::wgtFilesSearch(const QString &path, QWidget *parent) : QDialog(parent)
{
    this->resize(450, 170);
    this->setWindowTitle("File search");
    this->setModal(true);

    if (path.isEmpty())
        return;
    else
        dirPath = path;

    m_searchFileLbl = new QLabel(tr("Files:"));
    m_pathToFileLbl = new QLabel(tr("Path:"));
    m_searchFileEdit = new QLineEdit();
    m_pathToFileEdit = new QLineEdit();
    m_pathToFileEdit->setText(dirPath);

    m_withTextLbl = new QLabel(tr("With text:"));
    m_withTextEdit = new QLineEdit();

    m_withTextOption = new QCheckBox();
    m_caseSensOption = new QCheckBox(tr("Case sensitive"));
    m_wholeWordsOption = new QCheckBox(tr("Only whole words"));

    m_upGridLayout = new QGridLayout();
    m_upGridLayout->addWidget(m_searchFileLbl, 0, 0);
    m_upGridLayout->addWidget(m_searchFileEdit, 0, 1);
    m_upGridLayout->addWidget(m_pathToFileLbl, 1, 0);
    m_upGridLayout->addWidget(m_pathToFileEdit, 1, 1);

    QFrame *lineOnWgt = new QFrame();
    lineOnWgt->setFrameStyle(QFrame::HLine | QFrame::Raised);
    m_upGridLayout->addWidget(lineOnWgt, 2, 0, 1, 4);

    m_upGridLayout->addWidget(m_withTextLbl, 3, 0);
    m_upGridLayout->addWidget(m_withTextEdit, 3, 1);
    m_upGridLayout->addWidget(m_withTextOption, 3, 2);
    m_upGridLayout->addWidget(m_caseSensOption, 4, 0);
    m_upGridLayout->addWidget(m_wholeWordsOption, 5, 0);

    //tab pages
    m_generalTab = new QWidget();
    m_generalTab->setLayout(m_upGridLayout);

    //tab
    m_tabWgt = new QTabWidget();
    m_tabWgt->addTab(m_generalTab, QString(tr("General")));
    m_tabWgt->addTab(new QLabel(tr("2")), QString(tr("Options")));

    m_upTopLayout = new QHBoxLayout;
    m_upTopLayout->addWidget(m_tabWgt);

    m_rightBtnLayout = new QVBoxLayout();
    m_btnSearch = new QPushButton(tr("Search"));
    m_btnCancel = new QPushButton(tr("Cancel"));
    m_rightBtnLayout->addSpacing(20);
    m_rightBtnLayout->addWidget(m_btnSearch);
    m_rightBtnLayout->addWidget(m_btnCancel);
    m_rightBtnLayout->addStretch();
    m_upTopLayout->addLayout(m_rightBtnLayout);

    m_mainBoxLayout = new QVBoxLayout();
    m_foundFileList = new QListWidget();
    m_foundFileList->addItem("d:/");
    //listWgt->setVisible(false);
    m_mainBoxLayout->addLayout(m_upTopLayout);
    m_searchPathLbl = new QLabel(tr(":"));

    m_mainBoxLayout->addWidget(m_searchPathLbl);
    m_mainBoxLayout->addWidget(m_foundFileList);

    m_bottomBtnLayout = new QHBoxLayout();
    m_btnViewFile = new QPushButton(tr("View"));
    m_btnGotoFile = new QPushButton(tr("Go to file"));
    m_btnNewSearch = new QPushButton(tr("New Search"));
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
    connect(m_btnSearch, &QPushButton::pressed, this, &wgtFilesSearch::searchForFiles);
    connect (m_foundFileList, &QListWidget::itemDoubleClicked, this, &wgtFilesSearch::listOfFilesClicked);
}

void wgtFilesSearch::withTextOptionSlot(bool state)
{
    m_withTextEdit->setEnabled(state);
    m_caseSensOption->setEnabled(state);
    m_wholeWordsOption->setEnabled(state);
}

void wgtFilesSearch::searchForFiles()
{
    if (m_btnSearch->text() == "Search")
        m_btnSearch->setText(tr("Stop"));
    else
        m_btnSearch->setText(tr("Search"));
}

void wgtFilesSearch::listOfFilesClicked()
{
    emit(m_foundFileList->currentItem()->text());
}
