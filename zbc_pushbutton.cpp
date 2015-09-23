#include "zbc_pushbutton.h"

#include <QMouseEvent>


//C-tor
ZBC_PushButton::ZBC_PushButton(const QString& str, QWidget* pwgt) : QPushButton(str, pwgt)
{
    this->setFlat(true);
}


//Mouse above button
/*virtual*/ void ZBC_PushButton::enterEvent(QEvent *pe)
{
    this->setFlat(false);
    QPushButton::enterEvent(pe);
}


//Mouse laeve button area
/*virtual*/ void ZBC_PushButton::leaveEvent(QEvent *pe)
{
    this->setFlat(true);
    QPushButton::leaveEvent(pe);
}
