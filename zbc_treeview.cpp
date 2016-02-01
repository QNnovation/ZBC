#include "zbc_treeview.h"

#include <QPainter>
#include <QHeaderView>


//C-tor
ZBC_TreeView::ZBC_TreeView(QWidget* pwgt) : QTreeView(pwgt)
{
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QTreeView::NoEditTriggers);
    setItemDelegate(new ZBC_ItemDelegate(this));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QFont curFont = this->font();
    curFont.setBold(true);
    setFont(curFont);
}


void ZBC_TreeView::setColumnWidth(int c, int w)
{
    QTreeView::setColumnWidth(c, w);
}


void ZBC_TreeView::setColumnWidth()
{
    this->header()->setStretchLastSection(false);
    this->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    this->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    this->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    this->header()->setSectionResizeMode(0, QHeaderView::Stretch);
}


/*virtual*/void ZBC_TreeView::focusInEvent(QFocusEvent* pe)
{
    emit Active();
    QTreeView::focusInEvent(pe);
}


/*virtual*/void ZBC_TreeView::resizeEvent(QResizeEvent* pe)
{
    setColumnWidth();
    QTreeView::resizeEvent(pe);
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
