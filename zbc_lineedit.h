#ifndef ZBC_LINEEDIT_H
#define ZBC_LINEEDIT_H

#include <QLineEdit>

class ZBC_LineEdit : public QLineEdit
{
    Q_OBJECT

private:
    QPalette        m_pltBackground;

public:
    explicit ZBC_LineEdit(QWidget* pwgt);

protected:
    QString         m_strPath;
    QString         m_strTargetDir;
    bool            m_bOverText;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* pe) override;
    virtual void focusOutEvent(QFocusEvent* pe) override;
    virtual void keyPressEvent(QKeyEvent* pe) override;
    virtual void mouseMoveEvent(QMouseEvent* pe) override;
    virtual void mousePressEvent(QMouseEvent* pe) override;

signals:
    void pressedEnter(QString);
    void mouseClicked(QString);
};

#endif // ZBC_LINEEDIT_H
