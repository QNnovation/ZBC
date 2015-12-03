#ifndef ZBC_DRIVEBUTTONSWIDGET_H
#define ZBC_DRIVEBUTTONSWIDGET_H

#include <QWidget>

class ZBC_DriveButtonsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZBC_DriveButtonsWidget(const QStringList& lstPath, QWidget* pwgt = 0);

signals:
    void clicked(const QString&);
};

#endif // ZBC_DRIVEBUTTON_H
