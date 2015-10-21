#ifndef ZBC_DRIVEBUTTON_H
#define ZBC_DRIVEBUTTON_H

#include <QPushButton>

class ZBC_DriveButton : public QPushButton
{
public:
    explicit ZBC_DriveButton(QString sPath, QWidget* pwgt = 0);

protected:
    virtual void mousePressEvent(QMouseEvent* pe) override;

signals:
//    virtual voi
};

#endif // ZBC_DRIVEBUTTON_H
