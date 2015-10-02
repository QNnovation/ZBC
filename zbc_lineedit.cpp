#include <QDebug>

#include "zbc_lineedit.h"

#include <QKeyEvent>


//C-tor
ZBC_LineEdit::ZBC_LineEdit(QWidget* pwgt) : QLineEdit(pwgt)
{
    this->setStyleSheet("background-color: #cccccc");
}


//
void ZBC_LineEdit::mouseDoubleClickEvent(QMouseEvent *pe)
{
    this->setReadOnly(false);
    this->setStyleSheet("background-color: 0");
    m_strPath = this->text();
    this->setCursorPosition(m_strPath.length());

    QLineEdit::mouseDoubleClickEvent(pe);
}


//Focus out
void ZBC_LineEdit::focusOutEvent(QFocusEvent *pe)
{
    if (!this->isReadOnly()){


    this->setReadOnly(true);
    this->setStyleSheet("background-color: #cccccc");
    if (m_strPath != "")
        this->setText(m_strPath);
    }

    QLineEdit::focusOutEvent(pe);
}


//Check if Enter is pressed
void ZBC_LineEdit::keyPressEvent(QKeyEvent *pe)
{
    if ( pe->key() == Qt::Key_Return ){
        emit pressedEnter(m_strPath);
        this->setReadOnly(true);
        this->setStyleSheet("background-color: #cccccc");
    }

    QLineEdit::keyPressEvent(pe);
}
