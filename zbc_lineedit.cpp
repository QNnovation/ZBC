#include <QDebug>

#include "zbc_lineedit.h"

#include <QFontMetrics>
#include <QKeyEvent>
//#include <QPair>
#include <QPainter>

//C-tor
ZBC_LineEdit::ZBC_LineEdit(QWidget* pwgt) : QLineEdit(pwgt)
{
    m_Iter = 0;
    m_strText = this->text();
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
// IF cursor under the text
    if ( pe->x() <= fm.width(this->text()) ){

//Fill new list with folders
        QStringList folders = this->text().split("\\");
//        qDebug() << folders;

//Fill new list of pairs with full path of parent folders and it length in pexels
//        QList< QPair<QString, int> > lstPair;
        m_lstPair.clear();
        int tmpLength = 0;
        int slashLength = fm.width("\\");
        for( QString str : folders ){
//            lstPair.push_back( QPair<QString, int>(str, tmpLength += (fm.width(str)) + slashLength) );
            m_lstPair.push_back( QPair<QString, int>(str, tmpLength += (fm.width(str)) + slashLength) );
        }
//        lstPair.last().second -= slashLength;
        m_lstPair.last().second -= slashLength;

//        qDebug() << fm.width(this->text());
//        qDebug() << pe->x();
//        qDebug() << m_lstPair;

//Get full path of folder under mouse and set true to attribute m_bOverText for repaint widget
        int startX = 0;
        for( clstpair_Iter topIter = m_lstPair.begin();
//        for( QList< QPair<QString, int> >::const_iterator topIter = lstPair.begin();
//             topIter != lstPair.end();
             topIter != m_lstPair.end();
             ++topIter){
            if((pe->x() > startX) && ( pe->x() <= topIter->second )){
//                qDebug() << topIter->first;
                m_strTargetDir = QString();
//                for( QList< QPair<QString, int> >::const_iterator it = lstPair.begin();
                for( clstpair_Iter it = m_lstPair.begin();
                     it != topIter;
                     ++it){
                    m_strTargetDir += it->first;
                    m_strTargetDir += "\\";
                }
                m_strTargetDir += topIter->first;
                m_bOverText = true;
                m_Iter = topIter;
                this->repaint();
//                qDebug() << m_strTargetDir;
            }
            startX = topIter->second;
        }
    }
    else{
        m_bOverText = false;
        this->repaint();
//        qDebug() << "Out";
    }

    QLineEdit::mouseMoveEvent(pe);
}


//Pressed mouse over text
/*virtual*/ void ZBC_LineEdit::mousePressEvent(QMouseEvent *pe)
{
    if (m_bOverText){
//        qDebug() << m_strTargetDir;
        emit mouseClicked(m_strTargetDir);
    }

    QLineEdit::mouseMoveEvent(pe);
}


//HighLight text under mouse
/*virtual*/ void ZBC_LineEdit::paintEvent(QPaintEvent *pe)
{
/*
    if (m_bOverText){
        m_rect = pe->rect();
        m_pltBackground = this->palette();
        m_strPath = this->text();


        QBrush brush(Qt::red);
        QPainter painter(this);

        painter.setBrush(brush);
        painter.drawRect(m_rect);

        qDebug() << "paintEvent";
    }
    else{
        this->setPalette(m_pltBackground);
        this->setText(m_strText);
    }
*/
    QLineEdit::paintEvent(pe);
}



