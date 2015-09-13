#include <QDebug>

#include "zbccentralwidget.h"
#include "zbc_newfolder.h"
#include "zbc_sideframe.h"
#include "zbc_pushbutton.h"
#include "wgtcopy.h"
#include "wgttextview.h"

#include <QAction>
#include <QDirModel>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QSplitter>
#include <QSettings>

//C-tor
zbcCentralWidget::zbcCentralWidget(QWidget *pwgt) : QFrame(pwgt)
{
    createView();
    createActions();
    createConnections();
}


//D-tor
zbcCentralWidget::~zbcCentralWidget()
{
    m_psettings->beginGroup("/Settings");
    m_psettings->beginGroup("/Panel");

    m_psettings->setValue("/LeftPath", m_psfwLeft->getCurrentPath());
    m_psettings->setValue("/RightPath", m_psfwRight->getCurrentPath());

    m_psettings->endGroup();
    m_psettings->endGroup();
    delete m_psettings;
}


//Create Bottom Frame with buttons
void zbcCentralWidget::createBottomFrame()
{
//Frame
    m_pfrmBottomButtons = new QFrame;

//Bottom Buttons
    m_pbtnView          = new ZBC_PushButton(tr("F3 View"), this);
    m_pbtnEdit          = new ZBC_PushButton(tr("F4 Edit"), this);
    m_pbtnCopy          = new ZBC_PushButton(tr("F5 Copy"), this);
    m_pbtnMove          = new ZBC_PushButton(tr("F6 Move"), this);
    m_pbtnNewFolder     = new ZBC_PushButton(tr("F7 NewFolder"), this);
    m_pbtnDelete        = new ZBC_PushButton(tr("F8 Delete"), this);
    m_pbtnDelete->setFlat(true);
    m_pbtnExit          = new QPushButton(tr("Alt+F4 Exit"), this);

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
//Settings
    m_psettings          = new QSettings;
    m_psettings->beginGroup("/Settings");
    m_psettings->beginGroup("/Panel");

    if ( !m_psettings->contains("/LeftPath") )
        m_psettings->setValue("/LeftPath", "C:");

    if ( !m_psettings->contains("/RightPath") )
        m_psettings->setValue("/RightPath", "C:");


//Side Frames
    m_psfwLeft      = new ZBC_SideFrame((m_psettings->value("/LeftPath", "").toString()), this);
    m_psfwRight     = new ZBC_SideFrame((m_psettings->value("/RightPath", "").toString()), this);

    m_psettings->endGroup();
    m_psettings->endGroup();

//Splitter
    m_psplCentral               = new QSplitter(Qt::Horizontal);
    m_psplCentral->addWidget(m_psfwLeft);
    m_psplCentral->addWidget(m_psfwRight);
    m_psplCentral->setChildrenCollapsible(false);

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
    connect(m_psfwLeft, SIGNAL(Active(ZBC_SideFrame*)),
            this, SLOT(setActiveFrame(ZBC_SideFrame*)));

    connect(m_psfwRight, SIGNAL(Active(ZBC_SideFrame*)),
            this, SLOT(setActiveFrame(ZBC_SideFrame*)));

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
void zbcCentralWidget::setActiveFrame(ZBC_SideFrame* _psfw)
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
    pwgtTextView->loadFile(strFile);
    pwgtTextView->show();
}


//Edit button pressed
void zbcCentralWidget::runEdit()
{
    wgtTextView*    pwgtTextView        = new wgtTextView;
    QString strFile = m_psfwActive->getListOfSelectedItems().at(0);
    pwgtTextView->loadFile(strFile, 'w');
    pwgtTextView->show();
}


//Copy Buttom Pressed
void zbcCentralWidget::runCopy()
{
    FileOperationWgt* pwgtCopy               = new FileOperationWgt(this);
    QStringList lst =  m_psfwActive->getListOfSelectedItems();

    pwgtCopy->copyFileOperation(lst,
                       m_psfwNotActive->getCurrentPath());

    m_psfwActive->clearListOfSelectedItems();

    pwgtCopy->setModal(true);
    pwgtCopy->show();
//    pwgtCopy->exec();
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
    ZBC_NewFolder*  pwgtNewFolder   = new ZBC_NewFolder(m_psfwActive->getCurrentPath(), this);
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
