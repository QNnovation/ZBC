#ifndef ZBCPUSHBUTTON_H
#define ZBCPUSHBUTTON_H

#include <QPushButton>

class ZBC_PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ZBC_PushButton(const QString&, QWidget* pwgt = 0);

protected:
    virtual void enterEvent(QEvent* pe) override;
    virtual void leaveEvent(QEvent* pe) override;
};

#endif // ZBCPUSHBUTTON_H
