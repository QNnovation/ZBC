#ifndef ZBCPUSHBUTTON_H
#define ZBCPUSHBUTTON_H

#include <QPushButton>

class zbcPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit zbcPushButton(const QString&);

protected:
    virtual void enterEvent(QEvent* pe) override;
    virtual void leaveEvent(QEvent* pe) override;
};

#endif // ZBCPUSHBUTTON_H
