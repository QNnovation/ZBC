#ifndef ZBC_FILESYSTEMMODEL_H
#define ZBC_FILESYSTEMMODEL_H

#include <QFileSystemModel>

class ZBC_FileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit ZBC_FileSystemModel(QObject* pobj = 0);

protected:
    virtual QVariant data(const QModelIndex&, int) const override;
};

#endif // ZBC_FILESYSTEMMODEL_H
