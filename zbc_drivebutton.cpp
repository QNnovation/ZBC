#include <QDebug>

#include "zbc_drivebutton.h"

#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>


//C-tor
ZBC_DriveButton::ZBC_DriveButton(const QStringList& lstPath, QWidget* pwgt) : QWidget(pwgt)
{
    QSignalMapper* psglMapper   = new QSignalMapper(this);
    QHBoxLayout* pLayout        = new QHBoxLayout(this);

    for( QString sPath : lstPath ){
        QPushButton* pBtn   = new QPushButton(sPath, this);
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
            &ZBC_DriveButton::clicked);

    setLayout(pLayout);
}



