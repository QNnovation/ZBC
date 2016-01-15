#include <QDebug>

#include "zbc_lineedit.h"

#include <QFontMetrics>
#include <QKeyEvent>


//C-tor
ZBC_LineEdit::ZBC_LineEdit(QWidget* pwgt) : QLineEdit(pwgt)
{
    m_pltBackground.setColor(QPalette::Base, QColor(192, 192, 192));
    this->setPalette(m_pltBackground);
    QFont curFont = this->font();
    curFont.setBold(true);
    this->setFont(curFont);
}


//
void ZBC_LineEdit::mouseDoubleClickEvent(QMouseEvent *pe)
{
    this->setReadOnly(false);
    m_pltBackground.setColor(QPalette::Base, QColor(255, 255, 255));
    this->setPalette(m_pltBackground);
    m_strPath = this->text();
    this->setCursorPosition(m_strPath.length());

    QLineEdit::mouseDoubleClickEvent(pe);
}


//Focus out
void ZBC_LineEdit::focusOutEvent(QFocusEvent *pe)
{
    if (!this->isReadOnly()){
        this->setReadOnly(true);
        m_pltBackground.setColor(QPalette::Base, QColor(192, 192, 192));
        this->setPalette(m_pltBackground);
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
        m_pltBackground.setColor(QPalette::Base, QColor(192, 192, 192));
        this->setPalette(m_pltBackground);
    }

    QLineEdit::keyPressEvent(pe);
}


//Get mouse position and calculate it's posituin under text
/*virtual*/ void ZBC_LineEdit::mouseMoveEvent(QMouseEvent *pe)
{
    QFontMetrics fm(this->font());
    if ( pe->x() <= fm.width(this->text()) ){
        QStringList folders = this->text().split("\\");
        qDebug() << folders;
        QList< QPair<QString, int> > lstPair;
        int tmpLength = 0;
        int slashLength = fm.width("\\");
        for( QString str : folders ){
            lstPair.push_back( QPair<QString, int>(str, tmpLength += (fm.width(str)) + slashLength) );
        }
        lstPair.last().second -= slashLength;
        qDebug() << fm.width(this->text());
        qDebug() << pe->x();
        qDebug() << lstPair;
    }
    else{
        qDebug() << "Out";
    }

    QLineEdit::mouseMoveEvent(pe);
}
