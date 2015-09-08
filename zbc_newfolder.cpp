#include "zbc_newfolder.h"

#include <QDebug>

#include <QDialogButtonBox>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

ZBC_NewFolder::ZBC_NewFolder(QString _curPath, QWidget* pwgt) :
                            QDialog(pwgt),
                            sCurPath(_curPath)
{
    QLabel* plblText                = new QLabel("New folder(directory)");

    m_pledName                      = new QLineEdit;
    m_pledName->setPlaceholderText("New Folder");

    QDialogButtonBox* pdbbButtons   = new QDialogButtonBox(QDialogButtonBox::Ok
                                                           | QDialogButtonBox::Cancel);

    QVBoxLayout* pLayout            = new QVBoxLayout;
    pLayout->addWidget(plblText);
    pLayout->addWidget(m_pledName);
    pLayout->addWidget(pdbbButtons);
    this->setLayout(pLayout);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(pdbbButtons, &QDialogButtonBox::accepted, this, &ZBC_NewFolder::accepted);
    connect(pdbbButtons, &QDialogButtonBox::rejected, this, &ZBC_NewFolder::rejected);
}


void ZBC_NewFolder::accepted()
{
    QString sNewFolder = m_pledName->text();
    if(sNewFolder == "")
        sNewFolder = sCurPath + "New Folder";
    else
        sNewFolder = sCurPath + sNewFolder;

    if(!QDir(sNewFolder).exists()){
        if(!QDir("").mkdir(sNewFolder)){
            QMessageBox errorMsg(QMessageBox::Critical,
                                 QString("ZBC"),
                                 QString("Eror creation of " + sNewFolder),
                                 QMessageBox::Ok);
            errorMsg.exec();
        }
    }
    else{
        QMessageBox errorMsg(QMessageBox::Critical,
                             QString("ZBC"),
                             QString("Directory " + sNewFolder + " exists"),
                             QMessageBox::Ok);
        errorMsg.exec();
    }

    this->close();
}


void ZBC_NewFolder::rejected()
{
    this->close();
}
