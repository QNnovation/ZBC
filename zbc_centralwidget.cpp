#include "zbc_centralwidget.h"
#include "zbc_centralwidget_p.h"
#include "zbc_newfolder.h"

#include "wgtcopy.h"
#include "wgtfilessearch.h"
#include "wgttextview.h"

#include <QAction>
#include <QDirModel>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QKeyEvent>
#include <QSplitter>
#include <QSettings>
#include <QVBoxLayout>


//C-tor
ZBC_CentralWidget::ZBC_CentralWidget(QWidget* pwgt) :
    QFrame(pwgt),
    d_ptr(new ZBC_CentralWidgetPrivate(this))
{
}


//D-tor
ZBC_CentralWidget::~ZBC_CentralWidget()
{
}


//Override keyPressEvent
void ZBC_CentralWidget::keyPressEvent(QKeyEvent* pe)
{
    d_ptr->shiftDeletePressed(pe);
    QFrame::keyPressEvent(pe);
}



//C-tor
ZBC_CentralWidgetPrivate::ZBC_CentralWidgetPrivate(ZBC_CentralWidget* parent) :
    q_ptr(parent),
    m_pvblLayout( new QVBoxLayout(q_ptr) )
{
    Q_Q(ZBC_CentralWidget);

//Widgets
//Splitter
    m_psplCentral = new QSplitter(Qt::Horizontal, q);
    m_psplCentral->setChildrenCollapsible(false);

//Settings
    m_psettings                 = new QSettings(this);
    m_psettings->beginGroup("/Settings");
    m_psettings->beginGroup("/Panel");

    if ( !m_psettings->contains("/LeftPath") )
        m_psettings->setValue("/LeftPath", "C:");

    if ( !m_psettings->contains("/RightPath") )
        m_psettings->setValue("/RightPath", "C:");

//Side Frames
    m_psfwLeft      = new ZBC_SideFrame((m_psettings->value("/LeftPath", "").toString()), m_psplCentral);
    m_psfwRight     = new ZBC_SideFrame((m_psettings->value("/RightPath", "").toString()), m_psplCentral);
    m_psfwNotActive = m_psfwLeft;
    m_psfwActive = m_psfwRight;

    m_psplCentral->addWidget(m_psfwLeft);
    m_psplCentral->addWidget(m_psfwRight);

    m_psettings->endGroup();
    m_psettings->endGroup();

//Bottom Buttons Frame
    m_pfrmBottomButtons = new QFrame(q);
    m_pfrmBottomButtons->setFrameStyle(QFrame::Panel | QFrame::Plain);

//Bottom Buttons
    m_pbtnView      = new ZBC_PushButton("F3 View", m_pfrmBottomButtons);
    m_pbtnView->setFocusPolicy(Qt::NoFocus);
    m_pbtnEdit      = new ZBC_PushButton("F4 Edit", m_pfrmBottomButtons);
    m_pbtnEdit->setFocusPolicy(Qt::NoFocus);
    m_pbtnCopy      = new ZBC_PushButton("F5 Copy", m_pfrmBottomButtons);
    m_pbtnCopy->setFocusPolicy(Qt::NoFocus);
    m_pbtnMove      = new ZBC_PushButton("F6 Move", m_pfrmBottomButtons);
    m_pbtnMove->setFocusPolicy(Qt::NoFocus);
    m_pbtnNewFolder = new ZBC_PushButton("F7 NewFolder", m_pfrmBottomButtons);
    m_pbtnNewFolder->setFocusPolicy(Qt::NoFocus);
    m_pbtnDelete    = new ZBC_PushButton("F8 Delete", m_pfrmBottomButtons);
    m_pbtnDelete->setFocusPolicy(Qt::NoFocus);
    m_pbtnExit      = new ZBC_PushButton("Alt+F4 Exit", m_pfrmBottomButtons);
    m_pbtnExit->setFocusPolicy(Qt::NoFocus);

//Layout Buttons
    m_phblBoxLayout = new QHBoxLayout(m_pfrmBottomButtons);
    m_phblBoxLayout->setMargin(2);
    m_phblBoxLayout->addWidget(m_pbtnView);
    m_phblBoxLayout->addWidget(m_pbtnView);
    m_phblBoxLayout->addWidget(m_pbtnEdit);
    m_phblBoxLayout->addWidget(m_pbtnCopy);
    m_phblBoxLayout->addWidget(m_pbtnMove);
    m_phblBoxLayout->addWidget(m_pbtnNewFolder);
    m_phblBoxLayout->addWidget(m_pbtnDelete);
    m_phblBoxLayout->addWidget(m_pbtnExit);

    m_pfrmBottomButtons->setLayout(m_phblBoxLayout);

//Layout Top
    m_pvblLayout->setMargin(0);
    m_pvblLayout->addWidget(m_psplCentral,2);
    m_pvblLayout->addWidget(m_pfrmBottomButtons);

    q->setLayout(m_pvblLayout);


//Create Actions
//View
    m_pactView  = new QAction(q);
    m_pactView->setShortcut(QKeySequence(Qt::Key_F3));
    q->addAction(m_pactView);

//Edit
    m_pactEdit  = new QAction(q);
    m_pactEdit->setShortcut(QKeySequence(Qt::Key_F4));
    q->addAction(m_pactEdit);

//Copy
    m_pactCopy  = new QAction(q);
    m_pactCopy->setShortcut(QKeySequence(Qt::Key_F5));
    q->addAction(m_pactCopy);

//Move
    m_pactMove  = new   QAction(q);
    m_pactMove->setShortcut(QKeySequence(Qt::Key_F6));
    q->addAction(m_pactMove);

//New Folder
    m_pactNewFolder = new QAction(q);
    m_pactNewFolder->setShortcut(QKeySequence(Qt::Key_F7));
    q->addAction(m_pactNewFolder);

//Delete to trash
    m_pactDelete    = new QAction(q);
    QList<QKeySequence> shortcuts;
    shortcuts.push_back(QKeySequence(Qt::Key_F8));
    shortcuts.push_back(QKeySequence(Qt::Key_Delete));
    m_pactDelete->setShortcuts(shortcuts);
    q->addAction(m_pactDelete);

//Create Connections
//Active Side
    connect(m_psfwLeft,
            &ZBC_SideFrame::Active,
            [this](){
                    this->m_psfwActive = this->m_psfwLeft;
                    this->m_psfwNotActive = this->m_psfwRight;
                });

    connect(m_psfwRight,
        &ZBC_SideFrame::Active,
        [this](){
                this->m_psfwActive = this->m_psfwRight;
                this->m_psfwNotActive = this->m_psfwLeft;
                });

//Run View
    connect(m_pactView,
            &QAction::triggered,
            [this](){
                if ( QFileInfo(this->m_psfwActive->getListOfSelectedItems().at(0)).isFile() ){
                    wgtTextView* TextView   = new wgtTextView(qobject_cast<QWidget*>(this));
                    TextView->loadFile(this->m_psfwActive->getListOfSelectedItems().at(0));
                    TextView->show();
                }
            });

    connect(m_pbtnView,
            &ZBC_PushButton::clicked,
            [this](){
                if ( QFileInfo(this->m_psfwActive->getListOfSelectedItems().at(0)).isFile() ){
                    wgtTextView* TextView   = new wgtTextView(qobject_cast<QWidget*>(this));
                    TextView->loadFile(this->m_psfwActive->getListOfSelectedItems().at(0));
                    TextView->show();
                }
            });

//Run Edit
    connect(m_pactEdit,
            &QAction::triggered,
            [this](){
                if ( QFileInfo(this->m_psfwActive->getListOfSelectedItems().at(0)).isFile() ){
                    wgtTextView* TextView   = new wgtTextView(qobject_cast<QWidget*>(this));
                    TextView->loadFile(this->m_psfwActive->getListOfSelectedItems().at(0), 'w');
                    TextView->show();
                }
            });


    connect(m_pbtnEdit,
            &ZBC_PushButton::clicked,
            [this](){
                if ( QFileInfo(this->m_psfwActive->getListOfSelectedItems().at(0)).isFile() ){
                    wgtTextView* TextView   = new wgtTextView(qobject_cast<QWidget*>(this));
                    TextView->loadFile(this->m_psfwActive->getListOfSelectedItems().at(0), 'w');
                    TextView->show();
        }
            });

//Run Copy
    connect(m_pactCopy,
            &QAction::triggered,
            [this](){
                FileOperationWgt* wgtCopy   = new FileOperationWgt;
                wgtCopy->copyFileOperation(m_psfwActive->getListOfSelectedItems(),
                                           m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();
                wgtCopy->setModal(true);
                wgtCopy->show();
            });

    connect(m_pbtnCopy,
            &ZBC_PushButton::clicked,
            [this](){
                FileOperationWgt* wgtCopy   = new FileOperationWgt;
                wgtCopy->copyFileOperation( m_psfwActive->getListOfSelectedItems(),
                                            m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();
                wgtCopy->setModal(true);
                wgtCopy->show();
            });

//Move
    connect(m_pactMove,
            &QAction::triggered,
            [this](){
                FileOperationWgt* wgtMove   = new FileOperationWgt;
                wgtMove->moveFileOperation(m_psfwActive->getListOfSelectedItems(),
                                          m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();;
                wgtMove->setModal(true);
                wgtMove->show();
            });

    connect(m_pbtnMove,
            &ZBC_PushButton::clicked,
            [this](){
                FileOperationWgt* wgtMove   = new FileOperationWgt;
                wgtMove->moveFileOperation(m_psfwActive->getListOfSelectedItems(),
                                          m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();
                wgtMove->setModal(true);
                wgtMove->show();
            });

//Run New Folder
    connect(m_pactNewFolder,
            &QAction::triggered,
            [this](){
                ZBC_NewFolder* wgtNewFolder = new ZBC_NewFolder(m_psfwActive->getCurrentPath(), q_ptr);
                wgtNewFolder->exec();
            });

    connect(m_pbtnNewFolder,
            &ZBC_PushButton::clicked,
            [this](){
                ZBC_NewFolder* wgtNewFolder = new ZBC_NewFolder(m_psfwActive->getCurrentPath(), q_ptr);
                wgtNewFolder->exec();
            });

//Run Delete
    connect(m_pactDelete,
            &QAction::triggered,
            [this](){
                FileOperationWgt* wgtDelete = new FileOperationWgt;
                wgtDelete->moveToRecycleBin(m_psfwActive->getListOfSelectedItems());
                wgtDelete->setModal(true);
                wgtDelete->show();
            });

    connect(m_pbtnDelete,
            &ZBC_PushButton::clicked,
            [this](){
                FileOperationWgt* wgtDelete = new FileOperationWgt;
                wgtDelete->moveToRecycleBin(m_psfwActive->getListOfSelectedItems());
                wgtDelete->setModal(true);
                wgtDelete->show();
            });

//Exit
    connect(m_pbtnExit,
            &ZBC_PushButton::clicked,
            [q](){
            q->parentWidget()->close();
    });

//Signals From MainWindow
//Go Back
    connect(q_ptr,
            &ZBC_CentralWidget::goBack,
            m_psfwActive,
            &ZBC_SideFrame::goBack);

//Go Forward
    connect(q_ptr,
            &ZBC_CentralWidget::goForward,
            m_psfwActive,
            &ZBC_SideFrame::goForward);

/*
    connect(m_psfwActive,
            &ZBC_SideFrame::backAtEnd,
            q_ptr,
            &ZBC_CentralWidget::backAtEnd);
*/
}


//D-tor
ZBC_CentralWidgetPrivate::~ZBC_CentralWidgetPrivate()
{

    m_psettings->beginGroup("/Settings");
    m_psettings->beginGroup("/Panel");

    m_psettings->setValue("/LeftPath", m_psfwLeft->getCurrentPath());
    m_psettings->setValue("/RightPath", m_psfwRight->getCurrentPath());

    m_psettings->endGroup();
    m_psettings->endGroup();
    delete m_psettings;
}


//Invoked by ZBC_CentralWidget::keyPressedEvent(QKeyEvent* pe)
void ZBC_CentralWidgetPrivate::shiftDeletePressed(QKeyEvent *pe)
{
    switch (pe->key()) {
    case Qt::Key_Delete:
        if (pe->modifiers() & Qt::ShiftModifier){
            FileOperationWgt* wgtDelete = new FileOperationWgt;
            wgtDelete->removeFileOperation(m_psfwActive->getListOfSelectedItems());
            wgtDelete->setModal(true);
            wgtDelete->show();
        }
    case Qt::Key_F7:
        if (pe->modifiers() & Qt::AltModifier){
            wgtFilesSearch* wgtSearch   = new wgtFilesSearch(m_psfwActive->getCurrentPath(), q_ptr);
/*
            connect(wgtSearch,
                    &wgtFilesSearch::filePath,
                    [=](){
                        m_psfwActive->set
                    });
*/
            wgtSearch->show();
        }
        break;
    default:
        break;
    }
}
