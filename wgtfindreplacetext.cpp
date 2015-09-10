#include "wgtfindreplacetext.h"

#include <QLineEdit>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>

FindReplaceText::FindReplaceText(QWidget *parent)
    : QDialog(parent)
{
    //line edit block
    textFindEdit = new QLineEdit();
    textReplaceEdit = new QLineEdit();
    textFindLbl = new QLabel(tr("Find:"));
    textReplaceLbl = new QLabel(tr("Replace with:"));

    //radiobutton group
    directionLbl = new QLabel(tr("Direction"));
    downBtn = new QRadioButton(tr("Down"));
    upBtn = new QRadioButton(tr("Up"));

    //checkbox group
    optionsLbl = new QLabel(tr("Options"));
    caseSensitiveBox = new QCheckBox(tr("&Case sensitive"));
    wholeWordsBox = new QCheckBox(tr("&Whole words only"));
    regularExprBox = new QCheckBox(tr("&Regular Expression"));

    //buttons block
    findBtn = new QPushButton(tr("&Find"));
    closeBtn = new QPushButton(tr("&Close"));
    replaceBtn = new QPushButton(tr("&Replace"));
    replaceAllBtn = new QPushButton(tr("Replace &All"));
    replaceBtn->setVisible(false);
    replaceAllBtn->setVisible(false);

    statusLbl = new QLabel();

    QGridLayout *lineBlockLayout = new QGridLayout;
    lineBlockLayout->addWidget(textFindLbl, 0, 0);
    lineBlockLayout->addWidget(textFindEdit, 0, 1);
    lineBlockLayout->addWidget(findBtn, 0, 2);
    lineBlockLayout->addWidget(textReplaceLbl, 1, 0);
    lineBlockLayout->addWidget(textReplaceEdit, 1, 1);
    lineBlockLayout->addWidget(closeBtn, 1, 2);

    lineBlockLayout->addWidget(statusLbl, 2, 0);
    lineBlockLayout->addWidget(replaceBtn, 2, 2);

    lineBlockLayout->addWidget(directionLbl, 3, 0);
    lineBlockLayout->addWidget(optionsLbl, 3, 1);
    lineBlockLayout->addWidget(replaceAllBtn, 3, 2);
    lineBlockLayout->addWidget(downBtn, 4, 0);
    lineBlockLayout->addWidget(caseSensitiveBox, 4, 1);
    lineBlockLayout->addWidget(upBtn, 5, 0);
    lineBlockLayout->addWidget(wholeWordsBox, 5, 1);
    lineBlockLayout->addWidget(regularExprBox, 6, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(lineBlockLayout);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Find/Replace"));
    setFixedSize(380, 200);
    setLayout(mainLayout);

    connect(closeBtn, &QPushButton::clicked, this, &FindReplaceText::close);
}

FindReplaceText::~FindReplaceText()
{

}

