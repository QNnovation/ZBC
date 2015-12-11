#ifndef ZBC_DRIVEBUTTONSWIDGET_H
#define ZBC_DRIVEBUTTONSWIDGET_H

#include <QFrame>

class ZBC_DriveButtonsWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ZBC_DriveButtonsWidget(const QStringList& lstPath, QWidget* pwgt = 0);

public:
    static QIcon getIcon(const ushort*);

signals:
    void clicked(const QString&);
};

#endif // ZBC_DRIVEBUTTON_H
