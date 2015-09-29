#ifndef ZBC_CENTRALWIDGET_P_H
#define ZBC_CENTRALWIDGET_P_H

#include "zbc_sideframe.h"
#include "zbc_pushbutton.h"

class QFrame;
class QAction;
class QVBoxLayout;
class QHBoxLayout;
class QSettings;
class QSplitter;

class ZBC_CentralWidget;



class ZBC_CentralWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(ZBC_CentralWidget)

    ZBC_CentralWidget*  q_ptr;
public:
    explicit ZBC_CentralWidgetPrivate( ZBC_CentralWidget* parent);
    ~ZBC_CentralWidgetPrivate();

private:
    QVBoxLayout*        m_pvblLayout;
    QSplitter*          m_psplCentral;

    QSettings*          m_psettings;
    ZBC_SideFrame*      m_psfwLeft;
    ZBC_SideFrame*      m_psfwRight;
    ZBC_SideFrame*      m_psfwActive;
    ZBC_SideFrame*      m_psfwNotActive;


    QFrame*             m_pfrmBottomButtons;
    QHBoxLayout*        m_phblBoxLayout;
    ZBC_PushButton*     m_pbtnView;
    ZBC_PushButton*     m_pbtnEdit;
    ZBC_PushButton*     m_pbtnCopy;
    ZBC_PushButton*     m_pbtnMove;
    ZBC_PushButton*     m_pbtnNewFolder;
    ZBC_PushButton*     m_pbtnDelete;
    ZBC_PushButton*     m_pbtnExit;

    QAction*            m_pactView;
    QAction*            m_pactEdit;
    QAction*            m_pactCopy;
    QAction*            m_pactMove;
    QAction*            m_pactNewFolder;
    QAction*            m_pactDelete;
};

#endif // ZBC_CENTRALWIDGET_P_H

