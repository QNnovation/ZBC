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

FindReplaceText::FindReplaceText(QWidget *parent)
    : QDialog(parent)
{

    //line edit block
    m_textFindEdit = new QLineEdit();
    m_textReplaceEdit = new QLineEdit();
    m_textFindLbl = new QLabel(tr("Find:"));
    m_textReplaceLbl = new QLabel(tr("Replace with:"));
    m_textReplaceLbl->setVisible(false);
    m_textReplaceEdit->setVisible(false);

    //radiobutton group
    m_directionLbl = new QLabel(tr("Direction"));
    m_downBtn = new QRadioButton(tr("Down"));
    m_upBtn = new QRadioButton(tr("Up"));
    m_downBtn->setChecked(true);

    //checkbox group
    m_optionsLbl = new QLabel(tr("Options"));
    m_caseSensitiveBox = new QCheckBox(tr("&Case sensitive"));
    m_wholeWordsBox = new QCheckBox(tr("&Whole words only"));

    //buttons block
    m_findBtn = new QPushButton(tr("&Find"));
    m_closeBtn = new QPushButton(tr("&Close"));
    m_replaceBtn = new QPushButton(tr("&Replace"));
    m_replaceAllBtn = new QPushButton(tr("Replace &All"));
    m_replaceBtn->setVisible(false);
    m_replaceAllBtn->setVisible(false);

    m_statusLbl = new QLabel();

    QGridLayout *lineBlockLayout = new QGridLayout;
    lineBlockLayout->addWidget(m_textFindLbl, 0, 0);
    lineBlockLayout->addWidget(m_textFindEdit, 0, 1);
    lineBlockLayout->addWidget(m_findBtn, 0, 2);
    lineBlockLayout->addWidget(m_textReplaceLbl, 1, 0);
    lineBlockLayout->addWidget(m_textReplaceEdit, 1, 1);
    lineBlockLayout->addWidget(m_closeBtn, 1, 2);

    lineBlockLayout->addWidget(m_statusLbl, 2, 0);
    lineBlockLayout->addWidget(m_replaceBtn, 2, 2);

    lineBlockLayout->addWidget(m_directionLbl, 3, 0);
    lineBlockLayout->addWidget(m_optionsLbl, 3, 1);
    lineBlockLayout->addWidget(m_replaceAllBtn, 3, 2);
    lineBlockLayout->addWidget(m_downBtn, 4, 0);
    lineBlockLayout->addWidget(m_caseSensitiveBox, 4, 1);
    lineBlockLayout->addWidget(m_upBtn, 5, 0);
    lineBlockLayout->addWidget(m_wholeWordsBox, 5, 1);

    m_textReplaceLbl->setVisible(false);
    m_textReplaceEdit->setVisible(false);
    m_replaceBtn->setVisible(false);
    m_replaceAllBtn->setVisible(false);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(lineBlockLayout);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Find/Replace"));
    setFixedSize(380, 180);
    setLayout(mainLayout);

    connect(m_closeBtn, &QPushButton::clicked, this, &FindReplaceText::close);
    connect(m_findBtn, &QPushButton::clicked, this, &FindReplaceText::findSlot);
    connect(m_replaceBtn, &QPushButton::clicked, this, &FindReplaceText::replaceSlot);
    connect(m_replaceAllBtn, &QPushButton::clicked, this, &FindReplaceText::replaceAllSlot);
    connect(m_textFindEdit, &QLineEdit::textChanged, this, &FindReplaceText::isEmptyText);
}

void FindReplaceText::isReplace(bool mode)
{
    if (mode) {
        m_textReplaceLbl->setVisible(true);
        m_textReplaceEdit->setVisible(true);
        m_replaceBtn->setVisible(true);
        m_replaceAllBtn->setVisible(true);
    }
}

//find text slot
void FindReplaceText::findSlot()
{
    flags = options();
    m_newWord = m_textReplaceEdit->text();
    emit findSignal(m_textFindEdit->text(), flags);
}

//replace button slot
void FindReplaceText::replaceSlot()
{
    flags = options();
    m_newWord = m_textReplaceEdit->text();
    emit replaceSignal(m_textFindEdit->text(), flags, m_newWord);
}

//replaceAll button slot
void FindReplaceText::replaceAllSlot()
{
    flags = options();
    m_newWord = m_textReplaceEdit->text();
    emit replaceAllSignal(m_textFindEdit->text(), flags, m_newWord);
}

//block buttons if text is empty
void FindReplaceText::isEmptyText(const QString &text)
{
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    if (text.isEmpty()) {
        for (int i = 0; i < list.size(); ++i) {
            list.at(i)->setEnabled(false);
        }
    } else {
        for (int i = 0; i < list.size(); ++i) {
            list.at(i)->setEnabled(true);
        }
    }
}

QTextDocument::FindFlags FindReplaceText::options() const
{
    QTextDocument::FindFlags options;
    if (m_upBtn->isChecked())
        options = QTextDocument::FindBackward;
    else if (m_caseSensitiveBox->isChecked())
        options = QTextDocument::FindCaseSensitively;
    else if (m_caseSensitiveBox->isChecked() && m_upBtn->isChecked())
        options = QTextDocument::FindCaseSensitively | QTextDocument::FindBackward;
    else if (m_wholeWordsBox->isChecked())
        options = QTextDocument::FindWholeWords;
    else if (m_wholeWordsBox->isChecked() && m_upBtn->isChecked())
        options = QTextDocument::FindWholeWords | QTextDocument::FindBackward;
    else if (m_caseSensitiveBox->isChecked() && m_wholeWordsBox->isChecked() && m_upBtn->isChecked())
        options = QTextDocument::FindWholeWords | QTextDocument::FindCaseSensitively | QTextDocument::FindBackward;
    else if (m_caseSensitiveBox->isChecked() && m_wholeWordsBox->isChecked())
        options = QTextDocument::FindWholeWords | QTextDocument::FindCaseSensitively;
    return options;
}

//destructor
FindReplaceText::~FindReplaceText()
{

}


