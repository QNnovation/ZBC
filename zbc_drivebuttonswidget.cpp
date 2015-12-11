//#include <QDebug>

#include "zbc_drivebuttonswidget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QStorageInfo>

#include <windows.h>

//C-tor
ZBC_DriveButtonsWidget::ZBC_DriveButtonsWidget(const QStringList& lstPath, QWidget* pwgt) : QFrame(pwgt)
{
    QSignalMapper* psglMapper   = new QSignalMapper(this);
    QHBoxLayout* pLayout        = new QHBoxLayout(this);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);

    for( QString sPath : lstPath ){
        QPushButton* pBtn   = new QPushButton(getIcon(sPath.utf16()), sPath, this);
        pBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        QStorageInfo* pStorInfo = new QStorageInfo(sPath);
        pBtn->setToolTip(pStorInfo->displayName());
        delete pStorInfo;
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

    setFrameStyle(QFrame::Box | QFrame::Sunken);
    setFixedHeight(minimumSizeHint().height());
    setLayout(pLayout);
}


//Get type of volume and return QIcon
QIcon ZBC_DriveButtonsWidget::getIcon(const ushort* sDrive)
{
    uint type = GetDriveType( reinterpret_cast<LPCWSTR>(sDrive) );
    switch(type) {
        case DRIVE_REMOVABLE:
            return QIcon(":/buttons/drives/resource/usbdrive.ico");
        case DRIVE_FIXED:
            return QIcon(":/buttons/drives/resource/logicaldrive.ico");
        case DRIVE_REMOTE:
            return QIcon(":/buttons/drives/resource/networkdrive.ico");
        case DRIVE_CDROM:
            return QIcon(":/buttons/drives/resource/cddrive.ico");
        case DRIVE_RAMDISK:
            return QIcon(":/buttons/drives/resource/unknowndrive.ico");
        case DRIVE_UNKNOWN:
            return QIcon(":/buttons/drives/resource/unknowndrive.ico");
        case DRIVE_NO_ROOT_DIR:
            return QIcon(":/buttons/drives/resource/unknowndrive.ico");
        default:
            return QIcon(":/buttons/drives/resource/unknowndrive.ico");
    }

return QIcon(":/buttons/drives/resource/unknowndrive.ico");
}
