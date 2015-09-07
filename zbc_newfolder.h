#ifndef ZBC_NEWFOLDER_H
#define ZBC_NEWFOLDER_H

#include <QDialog>

class ZBC_NewFolder : public QDialog
{
    Q_OBJECT
public:
    explicit ZBC_NewFolder( QWidget* pwgt = 0 );

public slots:
    void createFolder(/*const QString&*/);

private slots:
    void  rejected();

signals:
    void accepted();
};

#endif // ZBC_NEWFOLDER_H
