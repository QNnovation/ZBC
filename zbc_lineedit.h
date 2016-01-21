#ifndef ZBC_LINEEDIT_H
#define ZBC_LINEEDIT_H

#include <QLineEdit>

typedef QList< QPair<QString, int> > lstPair;
typedef QList< QPair<QString, int> >::const_iterator clstpair_Iter ;

class ZBC_LineEdit : public QLineEdit
{
    Q_OBJECT

private:
    QPalette                        m_pltBackground;
    QTextCharFormat                 defaultFormat;
    QString                         m_strText;


    QString                         m_strPath;
    QString                         m_strTargetDir;
    bool                            m_bOverText;
    lstPair                         m_lstPair;
//    clstpair_Iter                   m_Iter;

public:
    explicit ZBC_LineEdit(QWidget* pwgt);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* pe) override;
    virtual void focusOutEvent(QFocusEvent* pe) override;
    virtual void keyPressEvent(QKeyEvent* pe) override;
    virtual void mouseMoveEvent(QMouseEvent* pe) override;
    virtual void mousePressEvent(QMouseEvent* pe) override;

    void highlightText(const QStringList&);
    void unHightlightText();

signals:
    void pressedEnter(QString);
    void mouseClicked(QString);
};

#endif // ZBC_LINEEDIT_H
