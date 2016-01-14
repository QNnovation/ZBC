#ifndef ZBC_TREEVIEW_H
#define ZBC_TREEVIEW_H

#include <QTreeView>
#include <QItemDelegate>

class ZBC_TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ZBC_TreeView(QWidget* pwgt = 0);
    void setColumnWidth(int, int);
    void setColumnWidth();

protected:
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void resizeEvent(QResizeEvent*) override;

signals:
    void Active();
};

class ZBC_ItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ZBC_ItemDelegate(QObject* pobj = 0) : QItemDelegate(pobj){}

protected:
    virtual void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
};

#endif // ZBC_TREEVIEW_H
