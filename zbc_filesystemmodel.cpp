#include "zbc_filesystemmodel.h"


//C-tor
ZBC_FileSystemModel::ZBC_FileSystemModel(QObject* pobj) : QFileSystemModel(pobj)
{}


/*virtual*/ QVariant ZBC_FileSystemModel::data(const QModelIndex& index, int role) const
{
    switch (index.column()){
    case 0:
    case 1:
    case 2:
    case 3:
        if(role == Qt::TextAlignmentRole)
            return Qt::AlignLeft;
        break;
    default:
        break;
    }

    return QFileSystemModel::data(index, role);
}
