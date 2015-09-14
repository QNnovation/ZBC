#include "zbc_centralwidget.h"
#include "zbc_centralwidget_p.h"
#include "zbc_newfolder.h"
#include "zbc_pushbutton.h"
#include "zbc_sideframe.h"
#include "wgtcopy.h"
#include "wgttextview.h"

#include <QAction>
#include <QDirModel>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QSplitter>
#include <QSettings>
#include <QVBoxLayout>

#include <QPushButton>

//C-tor
ZBC_CentralWidget::ZBC_CentralWidget(QWidget* pwgt) : QFrame(pwgt), d_ptr(new ZBC_CentralWidgetPrivate(this))
{
    Q_D(ZBC_CentralWidget);
    d->q_ptr = this;
    setLayout(d->pLayout);
}


//C-tor
ZBC_CentralWidgetPrivate::ZBC_CentralWidgetPrivate(ZBC_CentralWidget* parent) : q_ptr(parent)
{
    pbtn = new ZBC_PushButton("Test");
    pLayout = new QVBoxLayout;
    pLayout->addWidget(pbtn);

/*
//Widgets
    m_psettings                 = new QSettings;
    m_psettings->beginGroup("/Settings");
    m_psettings->beginGroup("/Panel");

    if ( !m_psettings->contains("/LeftPath") )
        m_psettings->setValue("/LeftPath", "C:");

    if ( !m_psettings->contains("/RightPath") )
        m_psettings->setValue("/RightPath", "C:");


//Side Frames
    m_psfwLeft                  = new ZBC_SideFrame((m_psettings->value("/LeftPath", "").toString()), q_ptr);
    m_psfwRight                 = new ZBC_SideFrame((m_psettings->value("/RightPath", "").toString()), q_ptr);

    m_psettings->endGroup();
    m_psettings->endGroup();


//Splitter
    QSplitter* splCentral = new QSplitter(Qt::Horizontal, q_ptr);
    splCentral->addWidget(m_psfwLeft);
    splCentral->addWidget(m_psfwRight);
    splCentral->setChildrenCollapsible(false);

    m_psfwNotActive = m_psfwLeft;
    m_psfwActive = m_psfwRight;

//Bottom Buttons Frame
    QFrame* frmBottomButtons = new QFrame(q_ptr);

//Bottom Buttons
    ZBC_PushButton btnView(tr("F3 View"), q_ptr);
    ZBC_PushButton btnEdit(tr("F4 Edit"), q_ptr);
    ZBC_PushButton btnCopy(tr("F5 Copy"), q_ptr);
    ZBC_PushButton btnMove(tr("F6 Move"), q_ptr);
    ZBC_PushButton btnNewFolder(tr("F7 NewFolder"), q_ptr);
    ZBC_PushButton btnDelete(tr("F8 Delete"), q_ptr);
    ZBC_PushButton btnExit(tr("Alt+F4 Exit"), q_ptr);

//Layout Buttons
    QHBoxLayout hblBoxLayout(frmBottomButtons);
    hblBoxLayout.addWidget(&btnView);
    hblBoxLayout.addWidget(&btnEdit);
    hblBoxLayout.addWidget(&btnCopy);
    hblBoxLayout.addWidget(&btnMove);
    hblBoxLayout.addWidget(&btnNewFolder);
    hblBoxLayout.addWidget(&btnDelete);
    hblBoxLayout.addWidget(&btnExit);

    frmBottomButtons->setLayout(&hblBoxLayout);

//Layout Top
    QVBoxLayout* VblLayout = new QVBoxLayout(q_ptr);
    VblLayout->setMargin(0);
    VblLayout->addWidget(splCentral);
    VblLayout->addWidget(frmBottomButtons);
    q_ptr->setLayout(VblLayout);


//Create Actions
//View
    QAction actView(q_ptr);
    actView.setShortcut(QKeySequence(Qt::Key_F3));
    q_ptr->addAction(&actView);

//Edit
    QAction actEdit(q_ptr);
    actEdit.setShortcut(QKeySequence(Qt::Key_F4));
    q_ptr->addAction(&actEdit);

//Copy
    QAction actCopy(q_ptr);
    actCopy.setShortcut(QKeySequence(Qt::Key_F5));
    q_ptr->addAction(&actCopy);

//Move
    QAction actMove(q_ptr);
    actMove.setShortcut(QKeySequence(Qt::Key_F6));
    q_ptr->addAction(&actMove);


//New Folder
    QAction actNewFolder(q_ptr);
    actNewFolder.setShortcut(QKeySequence(Qt::Key_F7));
    q_ptr->addAction(&actNewFolder);

//Delete
    QAction actDelete(q_ptr);
    actDelete.setShortcut(QKeySequence(Qt::Key_F8));
    q_ptr->addAction(&actDelete);


//Create Connections
//Active Side
    connect(m_psfwLeft,
            &ZBC_SideFrame::Active,
            [this](ZBC_SideFrame* _psfw){
                if (_psfw == this->m_psfwLeft){
                    this->m_psfwActive = this->m_psfwLeft;
                    this->m_psfwNotActive = this->m_psfwRight;
                }
                if (_psfw == this->m_psfwRight){
                    this->m_psfwActive = this->m_psfwRight;
                    this->m_psfwNotActive = this->m_psfwLeft;
                }});

    connect(m_psfwLeft,
        &ZBC_SideFrame::Active,
        [this](ZBC_SideFrame* _psfw){
            if (_psfw == this->m_psfwLeft){
                this->m_psfwActive = this->m_psfwLeft;
                this->m_psfwNotActive = this->m_psfwRight;
            }
            if (_psfw == this->m_psfwRight){
                this->m_psfwActive = this->m_psfwRight;
                this->m_psfwNotActive = this->m_psfwLeft;
                }});

//Run View
    connect(&actView,
            &QAction::triggered,
            [this](){
                wgtTextView wgtTextView(q_ptr);
                QString strFile = this->m_psfwActive->getListOfSelectedItems().at(0);
                wgtTextView.loadFile(strFile);
                wgtTextView.show();
            });
    connect(&btnView,
            &ZBC_PushButton::clicked,
            [this](){
                wgtTextView wgtTextView(q_ptr);
                QString strFile = this->m_psfwActive->getListOfSelectedItems().at(0);
                wgtTextView.loadFile(strFile);
                wgtTextView.show();
            });

//Run Edit
    connect(&actEdit,
            &QAction::triggered,
            [this](){
                wgtTextView TextView(q_ptr);
                QString strFile = m_psfwActive->getListOfSelectedItems().at(0);
                TextView.loadFile(strFile, 'w');
                TextView.show();
            });

    connect(&btnEdit,
            &ZBC_PushButton::clicked,
            [this](){
                wgtTextView TextView(q_ptr);
                QString strFile = m_psfwActive->getListOfSelectedItems().at(0);
                TextView.loadFile(strFile, 'w');
                TextView.show();
            });

//Run Copy
    connect(&actCopy,
            &QAction::triggered,
            [this](){
                FileOperationWgt wgtCopy(q_ptr);
                QStringList lst =  m_psfwActive->getListOfSelectedItems();
                wgtCopy.copyFileOperation(lst,
                                            m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();
                wgtCopy.setModal(true);
                wgtCopy.show();
            });

    connect(&btnCopy,
            &ZBC_PushButton::clicked,
            [this](){
                FileOperationWgt wgtCopy(q_ptr);
                QStringList lst =  m_psfwActive->getListOfSelectedItems();
                wgtCopy.copyFileOperation(lst,
                                            m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();
                wgtCopy.setModal(true);
                wgtCopy.show();
            });

//Move
    connect(&actMove,
            &QAction::triggered,
            [this](){
                FileOperationWgt wgtMove(q_ptr);
                QStringList lst = m_psfwActive->getListOfSelectedItems();
                wgtMove.moveFileOperation(lst,
                                          m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();;
                wgtMove.setModal(true);
                wgtMove.show();
            });

    connect(&btnMove,
            &ZBC_PushButton::clicked,
            [this](){
                FileOperationWgt wgtMove(q_ptr);
                QStringList lst = m_psfwActive->getListOfSelectedItems();
                wgtMove.moveFileOperation(lst,
                                          m_psfwNotActive->getCurrentPath());
                m_psfwActive->clearListOfSelectedItems();;
                wgtMove.setModal(true);
                wgtMove.show();
            });

//Run New Folder
    connect(&actNewFolder,
            &QAction::triggered,
            [this](){
                ZBC_NewFolder wgtNewFolder(m_psfwActive->getCurrentPath(), q_ptr);
                wgtNewFolder.exec();
            });

    connect(&btnNewFolder,
            &ZBC_PushButton::clicked,
            [this](){
                ZBC_NewFolder wgtNewFolder(m_psfwActive->getCurrentPath(), q_ptr);
                wgtNewFolder.exec();
            });

//Run Delete
    connect(&actDelete,
            &QAction::triggered,
            [this](){
                FileOperationWgt wgtDelete(q_ptr);
                QStringList lst =  m_psfwActive->getListOfSelectedItems();
                wgtDelete.removeFileOperation(lst);
                wgtDelete.setModal(true);
                wgtDelete.show();
            });

    connect(&btnDelete,
            &ZBC_PushButton::clicked,
            [this](){
                FileOperationWgt wgtDelete(q_ptr);
                QStringList lst =  m_psfwActive->getListOfSelectedItems();
                wgtDelete.removeFileOperation(lst);
                wgtDelete.setModal(true);
                wgtDelete.show();
            });

//Exit
    connect(&btnExit,
            &ZBC_PushButton::clicked,
            q_ptr,
            &ZBC_CentralWidget::close);
*/
}


//D-tor
ZBC_CentralWidgetPrivate::~ZBC_CentralWidgetPrivate()
{
/*
    m_psettings->beginGroup("/Settings");
    m_psettings->beginGroup("/Panel");

    m_psettings->setValue("/LeftPath", m_psfwLeft->getCurrentPath());
    m_psettings->setValue("/RightPath", m_psfwRight->getCurrentPath());

    m_psettings->endGroup();
    m_psettings->endGroup();
    delete m_psettings;
*/
}
