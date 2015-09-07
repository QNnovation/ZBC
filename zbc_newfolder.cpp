#include "zbc_newfolder.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

ZBC_NewFolder::ZBC_NewFolder(QWidget* pwgt) : QDialog(pwgt)
{
    QLabel* plblText                = new QLabel("New folder(directory)");

    QLineEdit* pledName             = new QLineEdit;
    pledName->setPlaceholderText("New Folder");

    QDialogButtonBox* pdbbButtons   = new QDialogButtonBox(QDialogButtonBox::Ok
                                                           | QDialogButtonBox::Cancel);

    QVBoxLayout* pLayout            = new QVBoxLayout;
    pLayout->addWidget(plblText);
    pLayout->addWidget(pledName);
    pLayout->addWidget(pdbbButtons);
    this->setLayout(pLayout);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(pdbbButtons, &QDialogButtonBox::accepted, this, &ZBC_NewFolder::accepted);
    connect(pdbbButtons, &QDialogButtonBox::rejected, this, &ZBC_NewFolder::rejected);
}


void ZBC_NewFolder::createFolder(/*const QString& _path*/)
{
    this->close();
}


void ZBC_NewFolder::rejected()
{
    this->close();
}
