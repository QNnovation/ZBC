#include "zbc_treeview.h"

#include <QPainter>


//C-tor
ZBC_TreeView::ZBC_TreeView(QWidget* pwgt) : QTreeView(pwgt)
{
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QTreeView::NoEditTriggers);
    setItemDelegate(new ZBC_ItemDelegate(this));
}


/*virtual*/void ZBC_TreeView::focusInEvent(QFocusEvent *pe)
{
    emit Active();
    QTreeView::focusInEvent(pe);
}


//Paint grid
/*virtual*/ void ZBC_ItemDelegate::paint(QPainter* pPainter,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
    if (index.row() % 2 != 0){
        QRect rect = option.rect;
        QBrush brush(Qt::lightGray);
        QPen pen(Qt::lightGray);
        pPainter->setBrush(brush);
        pPainter->setPen(pen);
        pPainter->drawRect(rect);
    }

    QItemDelegate::paint(pPainter, option, index);
}
