#include <QDebug>

#include "zbccentralwidget.h"
#include "zbcnewfolder.h"
#include "zbc_newfolder.h"
#include "zbcsideframe.h"
#include "zbcpushbutton.h"
#include "wgtcopy.h"
#include "wgttextview.h"

#include <QAction>
#include <QDirModel>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QSplitter>


//C-tor
zbcCentralWidget::zbcCentralWidget(QWidget *pwgt) : QFrame(pwgt)
{
    createView();
    createActions();
    createConnections();
}


//D-tor
zbcCentralWidget::~zbcCentralWidget(){}


//Create Bottom Frame with buttons
void zbcCentralWidget::createBottomFrame()
{
//Frame
    m_pfrmBottomButtons = new QFrame;

//Bottom Buttons
    m_pbtnView          = new zbcPushButton(tr("F3 View"));
    m_pbtnEdit          = new zbcPushButton(tr("F4 Edit"));
    m_pbtnCopy          = new zbcPushButton(tr("F5 Copy"));
    m_pbtnMove          = new zbcPushButton(tr("F6 Move"));
    m_pbtnNewFolder     = new zbcPushButton(tr("F7 NewFolder"));
    m_pbtnDelete        = new zbcPushButton(tr("F8 Delete"));
    m_pbtnDelete->setFlat(true);
    m_pbtnExit          = new QPushButton(tr("Alt+F4 Exit"));

//Layout
    QHBoxLayout*        phblBoxLayout = new QHBoxLayout(m_pfrmBottomButtons);
    phblBoxLayout->addWidget(m_pbtnView);
    phblBoxLayout->addWidget(m_pbtnEdit);
    phblBoxLayout->addWidget(m_pbtnCopy);
    phblBoxLayout->addWidget(m_pbtnMove);
    phblBoxLayout->addWidget(m_pbtnNewFolder);
    phblBoxLayout->addWidget(m_pbtnDelete);
    phblBoxLayout->addWidget(m_pbtnExit);

    m_pfrmBottomButtons->setLayout(phblBoxLayout);
}


void zbcCentralWidget::createView()
{
//Side Frames
    m_psfwLeft                  = new zbcSideFrame(this);
    m_psfwRight                 = new zbcSideFrame(this);

//Splitter
    m_psplCentral               = new QSplitter(Qt::Horizontal);
    m_psplCentral->addWidget(m_psfwLeft);
    m_psplCentral->addWidget(m_psfwRight);
    m_psplCentral->setChildrenCollapsible(false);

//Watcher
    m_pfsWatcher        = new QFileSystemWatcher;
    m_psfwNotActive = m_psfwLeft;

//Bottom Buttons Frame
    createBottomFrame();

//Layout
    QVBoxLayout* pVBoxLayout    = new QVBoxLayout;
    pVBoxLayout->setMargin(0);
    pVBoxLayout->addWidget(m_psplCentral,2);
    pVBoxLayout->addWidget(m_pfrmBottomButtons);

    setLayout(pVBoxLayout);
}


//Connections
void zbcCentralWidget::createConnections()
{
//Active Frame
    connect(m_psfwLeft, SIGNAL(Active(zbcSideFrame*)),
            this, SLOT(setActiveFrame(zbcSideFrame*)));

    connect(m_psfwRight, SIGNAL(Active(zbcSideFrame*)),
            this, SLOT(setActiveFrame(zbcSideFrame*)));

//View File
    connect(m_pbtnView, &QPushButton::clicked,
            this, &zbcCentralWidget::runView);
    connect(m_pactView, &QAction::triggered,
            this, &zbcCentralWidget::runView);

//Edit File
    connect(m_pbtnEdit, &QPushButton::clicked,
            this, &zbcCentralWidget::runEdit);
    connect(m_pactEdit, &QAction::triggered,
            this, &zbcCentralWidget::runEdit);

//Copy
    connect(m_pbtnCopy, SIGNAL(clicked()),
            this, SLOT(runCopy()));
    connect(m_pactCopy, SIGNAL(triggered()),
            this, SLOT(runCopy()));

//Move
    connect(m_pbtnMove, SIGNAL(clicked()),
            this, SLOT(runMove()));
    connect(m_pactMove, SIGNAL(triggered()),
            this, SLOT(runMove()));

//New Folder
    connect(m_pbtnNewFolder,SIGNAL(clicked()),
            this, SLOT(runNewFolder()));
    connect(m_pactNewFolder, SIGNAL(triggered()),
            this, SLOT(runNewFolder()));

//Delete
    connect(m_pbtnDelete, SIGNAL(clicked()),
            this, SLOT(runDelete()));
    connect(m_pactDelete, SIGNAL(triggered()),
            this, SLOT(runDelete()));

//Watcher
//    connect(m_pfsWatcher, SIGNAL(directoryChanged(QString)),
//            m_psfwNotActive, SLOT(setRootPathForFS(QString)));
}



void zbcCentralWidget::createActions()
{
//View
    m_pactView              = new QAction(this);
    m_pactView->setShortcut(QKeySequence(Qt::Key_F3));
    this->addAction(m_pactView);

//Edit
    m_pactEdit              = new QAction(this)   ;
    m_pactEdit->setShortcut(QKeySequence(Qt::Key_F4));
    this->addAction(m_pactEdit);

//Copy
    m_pactCopy              = new QAction(this);
    m_pactCopy->setShortcut(QKeySequence(Qt::Key_F5));
    this->addAction(m_pactCopy);

//Move
    m_pactMove              = new QAction(this);
    m_pactMove->setShortcut(QKeySequence(Qt::Key_F6));
    this->addAction(m_pactMove);

//New Folder
    m_pactNewFolder         = new QAction(this);
    m_pactNewFolder->setShortcut(QKeySequence(Qt::Key_F7));
    this->addAction(m_pactNewFolder);

//Delete
    m_pactDelete            = new QAction(this);
    m_pactDelete->setShortcut(QKeySequence(Qt::Key_F8));
    this->addAction(m_pactDelete);
}


//Set pointer to active frame
void zbcCentralWidget::setActiveFrame(zbcSideFrame* _psfw)
{
    if(_psfw == m_psfwLeft)
    {
        m_psfwActive = m_psfwLeft;
        m_psfwNotActive = m_psfwRight;
    }

    if(_psfw == m_psfwRight)
    {
        m_psfwActive = m_psfwRight;
        m_psfwNotActive = m_psfwLeft;
    }
}


//View button pressed
void zbcCentralWidget::runView()
{
    wgtTextView*    pwgtTextView        = new wgtTextView;
    QString strFile = m_psfwActive->getListOfSelectedItems().at(0);
    pwgtTextView->viewFile(strFile);
    pwgtTextView->show();
}


//Edit button pressed
void zbcCentralWidget::runEdit()
{
    wgtTextView*    pwgtTextView        = new wgtTextView;
    QString strFile = m_psfwActive->getListOfSelectedItems().at(0);
    pwgtTextView->editFile(strFile);
    pwgtTextView->show();
}


//Copy Buttom Pressed
void zbcCentralWidget::runCopy()
{

//qDebug() << "m_pfsWatcher->addPath(m_psfwNotActive->getCurrentPath()): " <<
//            m_pfsWatcher->addPath(m_psfwNotActive->getCurrentPath());

    FileOperationWgt* pwgtCopy               = new FileOperationWgt(this);
    QStringList lst =  m_psfwActive->getListOfSelectedItems();

    pwgtCopy->copyFileOperation(lst,
                       m_psfwNotActive->getCurrentPath());

    m_psfwActive->clearListOfSelectedItems();

    pwgtCopy->setModal(true);
    pwgtCopy->show();


//    connect(pwgtCopy, SIGNAL(finishOperation(QString)),
//            m_pfsWatcher, SIGNAL(directoryChanged(QString)));

//    connect(pwgtCopy, SIGNAL(finishOperation(QString)),
//            m_psfwNotActive, SLOT(setRootPathForFS(QString)));
}


void zbcCentralWidget::addFileToSystemWatcher()
{
    qDebug() << "m_pfsWatcher->addPath(\"D:/Test2/Forrest_Gump.avi\"): " <<
                 m_pfsWatcher->addPath("D:/Test2/Forrest_Gump.avi");

    emit fileAdded();
}


//Move button pressed
void zbcCentralWidget::runMove()
{
    FileOperationWgt* pwgtMove               = new FileOperationWgt(this);
    QStringList lst = m_psfwActive->getListOfSelectedItems();
    pwgtMove->moveFileOperation(lst,
                       m_psfwNotActive->getCurrentPath());
    m_psfwActive->clearListOfSelectedItems();;
    pwgtMove->setModal(true);
    pwgtMove->show();
}


//New Folder Button Pressed
void zbcCentralWidget::runNewFolder()
{
/*
    zbcNewFolder* pwgtNewFolder     = new zbcNewFolder(this);
    pwgtNewFolder->setModal(true);
    pwgtNewFolder->show();

    connect(pwgtNewFolder, SIGNAL(sendName(QString*)),
            m_psfwActive, SLOT(makeDir(QString*)));

    connect(m_psfwActive, SIGNAL(dirCreated()),
            pwgtNewFolder, SLOT(dirCreated()));

    connect(m_psfwActive, SIGNAL(dirNotCreated()),
            pwgtNewFolder, SLOT(dirNotCreated()));
*/
    ZBC_NewFolder*  pwgtNewFolder   = new ZBC_NewFolder(this);
//    pwgtNewFolder->setModal(true);
//    pwgtNewFolder->show();
    pwgtNewFolder->exec();
}


//Delete button pressed
void zbcCentralWidget::runDelete()
{
    FileOperationWgt* pwgtDelete               = new FileOperationWgt(this);
    QStringList lst =  m_psfwActive->getListOfSelectedItems();
    pwgtDelete->removeFileOperation(lst);
    pwgtDelete->setModal(true);
    pwgtDelete->show();
}
