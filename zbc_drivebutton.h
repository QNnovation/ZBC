#ifndef ZBC_DRIVEBUTTON_H
#define ZBC_DRIVEBUTTON_H

#include <QPushButton>

class ZBC_DriveButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ZBC_DriveButton(const QString&, QWidget* pwgt = 0);

protected:
//    virtual void paintEvent(QPaintEvent* pe) override;

};

#endif // ZBC_DRIVEBUTTON_H
