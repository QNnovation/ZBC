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


//C-tor
ZBC_CentralWidget::ZBC_CentralWidget(QWidget* pwgt) : QFrame(pwgt), d_ptr(new ZBC_CentralWidgetPrivate(this))
{

}


//C-tor
ZBC_CentralWidgetPrivate::ZBC_CentralWidgetPrivate(ZBC_CentralWidget* parent) : q_ptr(parent)
{
//Side Frames
    m_psfwLeft                  = new ZBC_SideFrame(QString("C:"), q_ptr);
    m_psfwRight                 = new ZBC_SideFrame(QString("D:"), q_ptr);

//Splitter
    QSplitter* psplCentral               = new QSplitter(Qt::Horizontal, q_ptr);
    psplCentral->addWidget(m_psfwLeft);
    psplCentral->addWidget(m_psfwRight);
    psplCentral->setChildrenCollapsible(false);

    m_psfwNotActive = m_psfwLeft;
}
