#include <QDebug>

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
        QPushButton* pBtn   = new QPushButton(sPath, this);
        pBtn->setFocusPolicy(Qt::NoFocus);
        pBtn->setFixedSize(pBtn->sizeHint());

/*
        QPalette palette;//(this->palette());
        palette.setBrush(QPalette::Button, QBrush(Qt::red));
        pBtn->setAutoFillBackground(true);
        pBtn->setPalette(palette);
*/

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
