#ifndef ZBCPUSHBUTTON_H
#define ZBCPUSHBUTTON_H

#include <QPushButton>

class zbcPushButton : public QPushButton
{
public:
    explicit zbcPushButton(const QString&);

protected:
    virtual void enterEvent(QEvent* pe);
    virtual void leaveEvent(QEvent* pe);
};

#endif // ZBCPUSHBUTTON_H
