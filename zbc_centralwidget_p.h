#ifndef ZBC_CENTRALWIDGET_P_H
#define ZBC_CENTRALWIDGET_P_H

#include "zbc_centralwidget.h"
//#include "zbc_sideframe.h"
#include "zbc_pushbutton.h"

class ZBC_SideFrame;

class QSettings;

class ZBC_CentralWidgetPrivate : QObject
{
    Q_OBJECT
public:
    explicit ZBC_CentralWidgetPrivate( ZBC_CentralWidget* parent);
    ~ZBC_CentralWidgetPrivate() {}

private:
    ZBC_CentralWidget*  q_ptr;
    Q_DECLARE_PUBLIC(ZBC_CentralWidget);
//MEMBERS
    ZBC_SideFrame*       m_psfwLeft;
    ZBC_SideFrame*       m_psfwRight;
    ZBC_SideFrame*       m_psfwActive;
    ZBC_SideFrame*       m_psfwNotActive;

    QFrame*             m_pfrmBottomButtons;
    ZBC_PushButton*      m_pbtnView;
    ZBC_PushButton*      m_pbtnEdit;
    ZBC_PushButton*      m_pbtnCopy;
    ZBC_PushButton*      m_pbtnMove;
    ZBC_PushButton*      m_pbtnNewFolder;
    ZBC_PushButton*      m_pbtnDelete;
    QPushButton*        m_pbtnExit;

    QAction*            m_pactView;
    QAction*            m_pactEdit;
    QAction*            m_pactCopy;
    QAction*            m_pactMove;
    QAction*            m_pactNewFolder;
    QAction*            m_pactDelete;

    QSettings*           m_psettings;

//METHODS
    void createBottomFrame();
    void createView();
    void createConnections();
    void createActions();

private slots:
/*
    void setActiveFrame(ZBC_SideFrame*);
    void runView();
    void runEdit();
    void runCopy();
    void runMove();
    void runNewFolder();
    void runDelete();
*/
signals:
    void fileAdded(/*QString*/);

};

#endif // ZBC_CENTRALWIDGET_P_H

