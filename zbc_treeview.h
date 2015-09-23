#ifndef ZBC_TREEVIEW_H
#define ZBC_TREEVIEW_H

#include<QTreeView>

class ZBC_TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ZBC_TreeView(QWidget* pwgt = 0);

protected:
    virtual void focusInEvent(QFocusEvent *event) override;

signals:
    void Active();
};

#endif // ZBC_TREEVIEW_H
