#ifndef ZBC_CENTRALWIDGET_P_H
#define ZBC_CENTRALWIDGET_P_H

//#include "zbc_centralwidget.h"

//class ZBC_SideFrame;
//class ZBC_PushButton;
//class QVBoxLayout;

//class QSettings;

#include <QLineEdit>
#include <QPushButton>

class ZBC_CentralWidget;

class ZBC_CentralWidgetPrivate // : QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(ZBC_CentralWidget)

    QScopedPointer<ZBC_CentralWidget>  q_ptr;

public:
    explicit ZBC_CentralWidgetPrivate( ZBC_CentralWidget* parent);
    ~ZBC_CentralWidgetPrivate();

private:
    QPushButton*        m_pbtn;
    QLineEdit*          m_pled;

/*
    ZBC_SideFrame*       m_psfwLeft;
    ZBC_SideFrame*       m_psfwRight;
    ZBC_SideFrame*       m_psfwActive;
    ZBC_SideFrame*       m_psfwNotActive;


    QSettings*           m_psettings;
*/
};

#endif // ZBC_CENTRALWIDGET_P_H

