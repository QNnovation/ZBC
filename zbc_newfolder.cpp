#include "zbc_newfolder.h"

#include <QDialogButtonBox>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

ZBC_NewFolder::ZBC_NewFolder(QString _curPath, QWidget* pwgt) : QDialog(pwgt)
{
    QLabel* plblText                = new QLabel("New folder(directory)", this);

    QLineEdit* pledName              = new QLineEdit(this);
    pledName->setPlaceholderText("New Folder");

    QDialogButtonBox* pdbbButtons   = new QDialogButtonBox(QDialogButtonBox::Ok
                                                           | QDialogButtonBox::Cancel,
                                                           this);

    QVBoxLayout* pLayout            = new QVBoxLayout(this);
    pLayout->addWidget(plblText);
    pLayout->addWidget(pledName);
    pLayout->addWidget(pdbbButtons);
    this->setLayout(pLayout);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->minimumSizeHint());

    connect(pdbbButtons, &QDialogButtonBox::accepted,
            [this, _curPath, pledName](){

                QString sNewFolder;
                if(pledName->text().isEmpty())
                    sNewFolder = _curPath + "New Folder";
                else
                    sNewFolder = _curPath + pledName->text();

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
    );

    connect(pdbbButtons, &QDialogButtonBox::rejected, [this](){ this->close(); });
}
