#ifndef ZBC_NEWFOLDER_H
#define ZBC_NEWFOLDER_H

#include <QDialog>

class ZBC_NewFolder : public QDialog
{
    Q_OBJECT
public:
    explicit ZBC_NewFolder( QWidget* pwgt = 0 );

private slots:
    void accepted();
    void  rejected();
};

#endif // ZBC_NEWFOLDER_H
