#include "zbc_lineedit.h"

#include <QCoreApplication>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QPainter>
#include <QTextLayout>

//C-tor
ZBC_LineEdit::ZBC_LineEdit(QWidget* pwgt) : QLineEdit(pwgt)
{
    this->setReadOnly(true);
    m_strText = this->text();
    m_pltBackground.setColor(QPalette::Base, QColor(192, 192, 192));
    this->setPalette(m_pltBackground);
    QFont curFont = this->font();
    curFont.setBold(true);
    defaultFormat = QTextCharFormat();
    this->setFont(curFont);
    this->installEventFilter(this);
}


//Override for enable input
/*virtual*/ void ZBC_LineEdit::mouseDoubleClickEvent(QMouseEvent *pe)
{
    this->setReadOnly(false);
    m_pltBackground.setColor(QPalette::Base, QColor(255, 255, 255));
    this->setPalette(m_pltBackground);
    m_strPath = this->text();
    this->setCursorPosition(m_strPath.length());

    QLineEdit::mouseDoubleClickEvent(pe);
}


//Override for processing unfinished input
/*virtual*/ void ZBC_LineEdit::focusOutEvent(QFocusEvent *pe)
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

//Fill new list of pairs with full path of parent folders and it length in pexels
        m_lstPair.clear();
        int tmpLength = 0;
        int slashLength = fm.width("\\");
        for( QString str : folders ){
            m_lstPair.push_back( QPair<QString, int>(str, tmpLength += (fm.width(str)) + slashLength) );
        }
        m_lstPair.last().second -= slashLength;

//Get full path of folder under mouse and set true to attribute m_bOverText for mousePressEvent
        int startX = 0;
        for( clstpair_Iter topIter = m_lstPair.begin();
             topIter != m_lstPair.end();
             ++topIter){
            QStringList lststrHighlight{};
            if((pe->x() > startX) && ( pe->x() <= topIter->second )){
                m_strTargetDir = QString();
                QString strSlash("\\");
                for( clstpair_Iter it = m_lstPair.begin();
                     it != topIter;
                     ++it){
                    m_strTargetDir += it->first;
                    m_strTargetDir += strSlash;
                }
                lststrHighlight.push_back(m_strTargetDir);
                m_strTargetDir += topIter->first;
                lststrHighlight.push_back(topIter->first + strSlash);
                m_bOverText = true;
                QString strLastPart{};
                clstpair_Iter it1 = topIter;
                while( ++it1 != m_lstPair.end()){
                        strLastPart += it1->first;
                        strLastPart += strSlash;
                }
                lststrHighlight.push_back(strLastPart);
                highlightText(lststrHighlight);
            }
            startX = topIter->second;
        }
    }
    else{
        unHightlightText();
        m_bOverText = false;
    }

    QLineEdit::mouseMoveEvent(pe);
}


//Pressed mouse over text
/*virtual*/ void ZBC_LineEdit::mousePressEvent(QMouseEvent *pe)
{
    if (m_bOverText){
        emit mouseClicked(m_strTargetDir);
    }

    QLineEdit::mouseMoveEvent(pe);
}


//
//Approach got there http://www.prog.org.ru/topic_8514_0.html
/*virtual*/ void ZBC_LineEdit::inputMethodEvent(QInputMethodEvent *pe)
{
    bool wasReadOnly = isReadOnly();
    if(wasReadOnly){
        setReadOnly(false);
        setCursor(defaultCursor);
    }

    QLineEdit::inputMethodEvent(pe);

    setReadOnly(wasReadOnly);
}


//
//Approach got there http://www.prog.org.ru/topic_8514_0.html
static void highlightEvent(const QList<QTextLayout::FormatRange>& lst, QLineEdit* led)
{
    QList<QInputMethodEvent::Attribute> attributes;
    for(QTextLayout::FormatRange formatRange : lst)
        attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat,
                                                       formatRange.start - led->cursorPosition(),
                                                       formatRange.length,
                                                       QVariant(formatRange.format)));

    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(led, &event);
}


//Highlight text under mouse
void ZBC_LineEdit::highlightText(const QStringList& lst)
{
    QList<QTextLayout::FormatRange> lstFormats{};
    QTextLayout::FormatRange formatRange;

    formatRange.start = 0;
    formatRange.length = lst.at(0).length();
    formatRange.format = defaultFormat;
    lstFormats.push_back(formatRange);

    QTextCharFormat textCharFormat;
    textCharFormat.setFontWeight(QFont::Bold);
    textCharFormat.setForeground(Qt::darkBlue);
    textCharFormat.setFontUnderline(true);
    formatRange.start = lst.at(0).length();
    formatRange.length = lst.at(1).length();
    formatRange.format = textCharFormat;
    lstFormats.push_back(formatRange);

    formatRange.start = lst.at(0).length() + lst.at(1).length();
    formatRange.length = lst.at(2).length();
    formatRange.format = defaultFormat;
    lstFormats.push_back(formatRange);

    highlightEvent(lstFormats, this);
}


//Disable highlighting text
void ZBC_LineEdit::unHightlightText()
{
    QList<QTextLayout::FormatRange> lstFormats{};
    QTextLayout::FormatRange formatRange;

    formatRange.start = 0;
    formatRange.length = this->text().length();
    formatRange.format = defaultFormat;
    lstFormats.push_back(formatRange);

    highlightEvent(lstFormats, this);
}


//Overrride for get mouse leave event and set default QTextCharFormat
/*virtual*/ bool ZBC_LineEdit::eventFilter(QObject *pobj, QEvent *pe)
{
    if (pe->type() == QEvent::Leave){
        unHightlightText();
        m_bOverText = false;
    }

    return QLineEdit::eventFilter(pobj, pe);
}










