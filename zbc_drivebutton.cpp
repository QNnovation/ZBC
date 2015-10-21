#include <QDebug>

#include "zbc_drivebutton.h"


//C-tor
ZBC_DriveButton::ZBC_DriveButton(QString sPath, QWidget* pwgt) : QPushButton(sPath, pwgt)
{}


//Overrrided
void ZBC_DriveButton::mousePressEvent(QMouseEvent *pe)
{
    this->clicked();
    QPushButton::mousePressEvent(pe);
}

