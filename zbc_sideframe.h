#ifndef ZBCSIDEFRAME_H
#define ZBCSIDEFRAME_H

#include <QFrame>

class ZBC_SideFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ZBC_SideFrame(const QString path, QWidget *pwgt = 0);
    ~ZBC_SideFrame();

    void clearListOfSelectedItems();
    QString getCurrentPath();
    QStringList getListOfSelectedItems() const;

private:
    QString                     m_sCurPath;
    QStringList                 stlSelectedItems;

signals:
    void Active();
};

#endif // ZBCSIDEFRAME_H

