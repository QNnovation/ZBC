#ifndef ZBC_LINEEDIT_H
#define ZBC_LINEEDIT_H

#include <QLineEdit>

class ZBC_LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ZBC_LineEdit(QWidget* pwgt);

protected:
    QString         m_strPath;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* pe) override;
    virtual void focusOutEvent(QFocusEvent* pe) override;
    virtual void keyPressEvent(QKeyEvent* pe) override;

signals:
    void pressedEnter(QString);
};

#endif // ZBC_LINEEDIT_H
