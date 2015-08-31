#ifndef ZBCNEWFOLDER_H
#define ZBCNEWFOLDER_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class zbcNewFolder : public QDialog
{
    Q_OBJECT

public:
    explicit zbcNewFolder(QWidget* pwgt = 0);
    ~zbcNewFolder();

private:
//MEMBERS
    QString*            m_pstrName;
    QLabel*             m_plblCaption;
    QLineEdit*          m_pledName;
    QPushButton*        m_pbtnOk;
    QPushButton*        m_pbtnCancel;

//METHODS
    void makeView();
    void makeConnections();

private slots:
    void setName();

public slots:
    void dirNotCreated();
    void dirCreated();

signals:
    void sendName(QString*);
};

#endif // ZBCNEWFOLDER_H
