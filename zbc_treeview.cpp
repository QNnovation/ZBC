#include "zbc_treeview.h"


//C-tor
ZBC_TreeView::ZBC_TreeView(QWidget* pwgt) : QTreeView(pwgt)
{
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QTreeView::NoEditTriggers);
}


/*virtual*/void ZBC_TreeView::focusInEvent(QFocusEvent *pe)
{
    emit Active();
    QTreeView::focusInEvent(pe);
}

