#include "zbc_drivebutton.h"
#include "zbc_drivebuttonswidget.h"

#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>


//C-tor
ZBC_DriveButtonsWidget::ZBC_DriveButtonsWidget(const QStringList& lstPath, QWidget* pwgt) : QWidget(pwgt)
{
    QSignalMapper* psglMapper   = new QSignalMapper(this);
    QHBoxLayout* pLayout        = new QHBoxLayout(this);


    for( QString sPath : lstPath ){
        ZBC_DriveButton* pBtn   = new ZBC_DriveButton(sPath, this);
        pBtn->setFocusPolicy(Qt::NoFocus);
        pBtn->setFixedSize(pBtn->sizeHint());
        connect(pBtn,
                &QPushButton::clicked,
                psglMapper,
                static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        psglMapper->setMapping(pBtn, sPath);
        pLayout->addWidget(pBtn);
    }

    connect(psglMapper,
            static_cast<void (QSignalMapper::*)(const QString&)>(&QSignalMapper::mapped),
            this,
            &ZBC_DriveButtonsWidget::clicked);

    setLayout(pLayout);
}



