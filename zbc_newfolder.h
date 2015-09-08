#ifndef ZBC_NEWFOLDER_H
#define ZBC_NEWFOLDER_H

#include <QDialog>

class QLineEdit;

class ZBC_NewFolder : public QDialog
{
private:
    Q_OBJECT
    QString         sCurPath;
    QLineEdit*      m_pledName;

public:
    explicit ZBC_NewFolder( QString, QWidget* pwgt = 0 );

private slots:
    void rejected();
    void accepted();
};

#endif // ZBC_NEWFOLDER_H
