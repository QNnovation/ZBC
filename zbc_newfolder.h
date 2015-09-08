#ifndef ZBC_NEWFOLDER_H
#define ZBC_NEWFOLDER_H

#include <QDialog>

class QLineEdit;

class ZBC_NewFolder : public QDialog
{
    Q_OBJECT

public:
    explicit ZBC_NewFolder( QString, QWidget* pwgt = 0 );
};

#endif // ZBC_NEWFOLDER_H
