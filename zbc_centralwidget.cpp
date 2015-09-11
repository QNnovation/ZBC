#include "zbc_centralwidget.h"
#include "zbc_centralwidget_p.h"
#include "zbc_newfolder.h"
#include "zbc_pushbutton.h"
#include "wgtcopy.h"
#include "wgttextview.h"

#include <QAction>
#include <QDirModel>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QSplitter>


//C-tor
ZBC_CentralWidget::ZBC_CentralWidget(QWidget* pwgt) : QFrame(pwgt), d_ptr(new ZBC_CentralWidgetPrivate)
{

}


//C-tor
ZBC_CentralWidgetPrivate::ZBC_CentralWidgetPrivate()
{
//Side Frames
    m_psfwLeft                  = new ZBC_SideFrame(this);
    m_psfwRight                 = new ZBC_SideFrame(this);

//Splitter
    psplCentral               = new QSplitter(Qt::Horizontal);
    m_psplCentral->addWidget(m_psfwLeft);
    m_psplCentral->addWidget(m_psfwRight);
    m_psplCentral->setChildrenCollapsible(false);

    m_psfwNotActive = m_psfwLeft;
}
