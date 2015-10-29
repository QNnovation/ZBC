#ifndef ZBC_DRIVEBUTTON_H
#define ZBC_DRIVEBUTTON_H

#include <QWidget>

class ZBC_DriveButton : public QWidget
{
    Q_OBJECT
public:
    explicit ZBC_DriveButton(const QStringList& lstPath, QWidget* pwgt = 0);

signals:
    void clicked(const QString&);
};

#endif // ZBC_DRIVEBUTTON_H
