#ifndef ZBC_CENTRALWIDGET_P_H
#define ZBC_CENTRALWIDGET_P_H

#include "zbc_centralwidget.h"

class ZBC_SideFrame;
class ZBC_PushButton;
class QVBoxLayout;

class QSettings;

class ZBC_CentralWidgetPrivate : QObject
{
    Q_OBJECT
public:
    explicit ZBC_CentralWidgetPrivate( ZBC_CentralWidget* parent);
    ~ZBC_CentralWidgetPrivate();

private:
    ZBC_CentralWidget*  q_ptr;
    Q_DECLARE_PUBLIC(ZBC_CentralWidget);

    ZBC_PushButton*     pbtn;
    QVBoxLayout*        pLayout;
/*
    ZBC_SideFrame*       m_psfwLeft;
    ZBC_SideFrame*       m_psfwRight;
    ZBC_SideFrame*       m_psfwActive;
    ZBC_SideFrame*       m_psfwNotActive;


    QSettings*           m_psettings;
*/
};

#endif // ZBC_CENTRALWIDGET_P_H

