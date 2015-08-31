#include "zbcnewfolder.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>


//C-tor
zbcNewFolder::zbcNewFolder(QWidget* pwgt) : QDialog(pwgt)
{
    makeView();
    makeConnections();
}


//D-tor
zbcNewFolder::~zbcNewFolder()
{
    delete m_pstrName;
    this->close();
}


//Create objects and layout it
void zbcNewFolder::makeView()
{
    m_pstrName                  = new QString("New Folder");
    m_plblCaption               = new QLabel(tr("New Folder(directory)"));
    m_pledName                  = new QLineEdit(*m_pstrName);
    m_pbtnOk                    = new QPushButton(tr("Ok"));
    m_pbtnCancel                = new QPushButton(tr("Cancel"));

    QGridLayout*    pLayout     = new QGridLayout;
    pLayout->addWidget(m_plblCaption, 0, 0, 1, 1);
    pLayout->addWidget(m_pledName, 1, 0, 1, 4);
    pLayout->addWidget(m_pbtnOk, 2, 2, 1, 1);
    pLayout->addWidget(m_pbtnCancel, 2, 3, 1, 1);
    this->setLayout(pLayout);
}


//Make connections
void zbcNewFolder::makeConnections()
{
    connect(m_pbtnOk, SIGNAL(clicked()),
            this, SLOT(setName()));

    connect(m_pbtnCancel, SIGNAL(clicked()),
            this, SLOT(dirCreated()));
}


//Get QString from QLineEdit
void zbcNewFolder::setName()
{
    *m_pstrName = m_pledName->text();
    emit sendName(m_pstrName);
}


//Dir NOT created: Qmessage box and delete it
void zbcNewFolder::dirNotCreated()
{
    QMessageBox mBox;
    mBox.setText("Directory Already Exists");
    delete this;
    mBox.exec();
}


//Dir create: close Diloag and delete it
void zbcNewFolder::dirCreated()
{
    delete this;
}
