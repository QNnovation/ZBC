#include "zbcpushbutton.h"

#include <QMouseEvent>


//C-tor
zbcPushButton::zbcPushButton(const QString& str) : QPushButton(str)
{
    this->setFlat(true);
}


//Mouse above button
/*virtual*/ void zbcPushButton::enterEvent(QEvent *pe)
{
    Q_UNUSED(pe)
    this->setFlat(false);
}


//Mouse laeve button area
/*virtual*/ void zbcPushButton::leaveEvent(QEvent *pe)
{
    Q_UNUSED(pe)
    this->setFlat(true);
}
