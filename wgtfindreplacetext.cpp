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
#include <QDebug>

FindReplaceText::FindReplaceText(bool mode, QWidget *parent)
    : QDialog(parent)
{

    //line edit block
    textFindEdit = new QLineEdit();
    textReplaceEdit = new QLineEdit();
    textFindLbl = new QLabel(tr("Find:"));
    textReplaceLbl = new QLabel(tr("Replace with:"));
    textReplaceLbl->setVisible(false);
    textReplaceEdit->setVisible(false);

    //radiobutton group
    directionLbl = new QLabel(tr("Direction"));
    downBtn = new QRadioButton(tr("Down"));
    upBtn = new QRadioButton(tr("Up"));
    downBtn->setChecked(true);

    //checkbox group
    optionsLbl = new QLabel(tr("Options"));
    caseSensitiveBox = new QCheckBox(tr("&Case sensitive"));
    wholeWordsBox = new QCheckBox(tr("&Whole words only"));

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

    if (mode)
    {
        textReplaceLbl->setVisible(true);
        textReplaceEdit->setVisible(true);
        replaceBtn->setVisible(true);
        replaceAllBtn->setVisible(true);
    }

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(lineBlockLayout);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Find/Replace"));
    setFixedSize(380, 180);
    setLayout(mainLayout);

    connect(closeBtn, &QPushButton::clicked, this, &FindReplaceText::close);
    connect(findBtn, &QPushButton::clicked, this, &FindReplaceText::findTextSlot);
    //connect(replaceBtn, &QPushButton::clicked, this, &FindReplaceText::);
}

void FindReplaceText::findTextSlot()
{
    QTextDocument::FindFlags options;
    if (upBtn->isChecked())
        options = QTextDocument::FindBackward;
    else if (caseSensitiveBox->isChecked())
        options = QTextDocument::FindCaseSensitively;
    else if (caseSensitiveBox->isChecked() && upBtn->isChecked())
        options = QTextDocument::FindCaseSensitively | QTextDocument::FindBackward;
    else if (wholeWordsBox->isChecked())
        options = QTextDocument::FindWholeWords;
    else if (wholeWordsBox->isChecked() && upBtn->isChecked())
        options = QTextDocument::FindWholeWords | QTextDocument::FindBackward;
    else if (caseSensitiveBox->isChecked() && wholeWordsBox->isChecked() && upBtn->isChecked())
        options = QTextDocument::FindWholeWords | QTextDocument::FindCaseSensitively | QTextDocument::FindBackward;
    else if (caseSensitiveBox->isChecked() && wholeWordsBox->isChecked())
        QTextDocument::FindWholeWords | QTextDocument::FindCaseSensitively;
    QString wordToReplace = textReplaceEdit->text();
    emit findTextOptionsSig(textFindEdit->text(), options, wordToReplace);
}

FindReplaceText::~FindReplaceText()
{

}


