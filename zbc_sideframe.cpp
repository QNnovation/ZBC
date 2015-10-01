#include "zbc_sideframe.h"
#include "zbc_treeview.h"

#include <QComboBox>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QUrl>


//C-tor
ZBC_SideFrame::ZBC_SideFrame(const QString path, QWidget *pwgt) : QFrame(pwgt)
{
//NTFS Permissions
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
    qt_ntfs_permission_lookup++;

//Model
    QFileSystemModel* pfsmModel             = new QFileSystemModel(this);
    pfsmModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
    pfsmModel->setRootPath(path);
    pfsmModel->setReadOnly(false);

//ProxyModel
    QSortFilterProxyModel* psfpModel        = new QSortFilterProxyModel(this);
    psfpModel->setSourceModel(pfsmModel);
    psfpModel->setSortCaseSensitivity(Qt::CaseInsensitive);

//TreeView
    ZBC_TreeView* ptreeView         = new ZBC_TreeView(this);
    ptreeView->setFrameStyle(QFrame::Box | QFrame::Raised);
    ptreeView->setModel(psfpModel);
    ptreeView->setSortingEnabled(true);
    ptreeView->sortByColumn(0, Qt::AscendingOrder);
    ptreeView->setRootIndex( psfpModel->mapFromSource(pfsmModel->index(path)) );

//ComboBox as View
    QComboBox*  pcbxVolumes     = new QComboBox(this);
    pcbxVolumes->setModel(psfpModel);
    pcbxVolumes->setMaximumSize(pcbxVolumes->sizeHint());

//Current path
    m_sCurPath = pfsmModel->rootPath();

//Label with current path
    QLabel* plblCurPath           = new QLabel(this);
    plblCurPath->setText(this->getCurrentPath());

//Label with info about files and dirs
    QLabel* plblDirInfo         = new QLabel(this);
    plblDirInfo->setFrameStyle(QFrame::Panel | QFrame::Plain);
    setListOfItemsInDir();
    setTextForLblDirInfo(plblDirInfo);

//Layout
    QGridLayout* pgrdLayout = new QGridLayout;
    pgrdLayout->setMargin(5);
    pgrdLayout->addWidget(pcbxVolumes, 0, 0, 1, 2);
    pgrdLayout->addWidget(plblCurPath, 1, 0, 1, 10);
    pgrdLayout->addWidget(ptreeView, 2, 0, 20, 20);
    pgrdLayout->addWidget(plblDirInfo, 22, 0, 1, 20);

    this->setLayout(pgrdLayout);

//Style
    this->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

//Connections
//Change value of QComboBox with drives
    connect(pcbxVolumes,
            static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            [this, pfsmModel, psfpModel, ptreeView, plblCurPath, plblDirInfo](QString _sPath){
                QString sCurDisk;
                if (_sPath.length() == 2)
                    sCurDisk = _sPath;
                else{
                    sCurDisk = _sPath.right(3);
                    sCurDisk.remove(')');
                }

                m_sCurPath = sCurDisk;
                plblCurPath->setText(getCurrentPath());
                ptreeView->setRootIndex(psfpModel->mapFromSource(pfsmModel->index(sCurDisk)));
                setTextForLblDirInfo(plblDirInfo);}
            );

//Double click on item at QTreeView(change dir or open file)
    connect(ptreeView,
            &QTreeView::doubleClicked,
            [this, pfsmModel, psfpModel, ptreeView, plblCurPath, plblDirInfo](const QModelIndex &_index){
                if (QFileInfo(pfsmModel->filePath(psfpModel->mapToSource(_index))).isDir()){
                    if ( (pfsmModel->permissions(psfpModel->mapToSource(_index))) == (QFlags<QFile::Permissions>(0)) )
                        QMessageBox::critical(this,
                                              "ZBC Error",
                                              "Access denied on folder " + pfsmModel->filePath(_index),
                                              QMessageBox::Ok);
                    else{
                        if ( (pfsmModel->filePath(psfpModel->mapToSource(_index))) == ".." )
                            ptreeView->setRootIndex(QModelIndex( psfpModel->mapFromSource(pfsmModel->index(".."))));
                        else{
                            ptreeView->setRootIndex(psfpModel->mapFromSource(pfsmModel->index(pfsmModel->filePath(psfpModel->mapToSource(_index)))));

                        }
                        m_sCurPath = pfsmModel->filePath( psfpModel->mapToSource(ptreeView->rootIndex()));
                        plblCurPath->setText(getCurrentPath());
                        stlSelectedItems.clear();
                        setTextForLblDirInfo(plblDirInfo);
                    }
                }
                else
                    QDesktopServices::openUrl(QUrl::fromLocalFile(pfsmModel->filePath(psfpModel->mapToSource(_index))));
            });

//Fill QStringList with selected items
    connect(ptreeView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            [this, pfsmModel, psfpModel, ptreeView, plblDirInfo](){
                QModelIndexList* plstIndexes = new QModelIndexList;
                *plstIndexes = ptreeView->selectionModel()->selectedRows();

                stlSelectedItems.clear();
                foreach (QModelIndex index, *plstIndexes){
                    if ( (pfsmModel->filePath(psfpModel->mapToSource(index)).right(2)) == ".." )
                        continue;
                    if ( pfsmModel->isDir(psfpModel->mapToSource(index)) )
                        stlSelectedItems.push_back(pfsmModel->filePath(psfpModel->mapToSource(index)) + "/");
                    else
                        stlSelectedItems.push_back(pfsmModel->filePath(psfpModel->mapToSource(index)));
                }
                plstIndexes->clear();
                setTextForLblDirInfo(plblDirInfo);
            });

//Send Signal about active TreeView
    connect(ptreeView,
            &ZBC_TreeView::Active,
            this,
            &ZBC_SideFrame::Active);
}


//D-tor
ZBC_SideFrame::~ZBC_SideFrame()
{
    delete m_psetDirs;
}


//Return list of selected files
QStringList ZBC_SideFrame::getListOfSelectedItems() const
{
    return stlSelectedItems;
}


//Clear QStringList with Selected Items
void ZBC_SideFrame::clearListOfSelectedItems()
{
    stlSelectedItems.clear();
}


//Return current path
QString ZBC_SideFrame::getCurrentPath()
{
    if(m_sCurPath.length() == 3)
        return m_sCurPath;
    return (m_sCurPath + "/");
}


//Number of files and dors in folder
void ZBC_SideFrame::setListOfItemsInDir()
{
    const int KBytes = 1024;
    QDir curFiles(m_sCurPath);
    curFiles.setFilter(QDir::Files
                     | QDir::NoSymLinks
                     | QDir::NoDotAndDotDot);

    QFileInfoList finList = curFiles.entryInfoList();
    foreach (QFileInfo file, finList)
        m_hashFiles.insert(file.absoluteFilePath(), file.size() / KBytes );


    QDir curDirs(m_sCurPath);
    curDirs.setFilter(QDir::Dirs
                     | QDir::NoSymLinks
                     | QDir::NoDotAndDotDot);

    QFileInfoList dirList = curDirs.entryInfoList();
    m_psetDirs             = new QSet<QString>;
    foreach (QFileInfo dir, dirList)
        m_psetDirs->insert(dir.absoluteFilePath() + "/");
}


//Get size of files saved in QHash dir
qint64 ZBC_SideFrame::getSizeOfFiles(QHash<QString, int> _hash) const
{
    int sz = 0;
    QHashIterator<QString, int> iter(_hash);
    while (iter.hasNext()){
        iter.next();
        sz += iter.value();
    }

    return sz;
}


//Calculate total size of selected files
//Stupid solution ( cause I don't now STL( )
qint64 ZBC_SideFrame::getSizeOfSelectedFiles( bool retSum )
{
    QHash<QString, int> tmpHash;
    QHashIterator<QString, int> iterHash(m_hashFiles);

    if ( stlSelectedItems.size() >= m_hashFiles.size() ){
        for (int i = 0; i != stlSelectedItems.size(); ++i){
            while(iterHash.hasNext()){
                iterHash.next();
                if ( stlSelectedItems.at(i) == iterHash.key() )
                    tmpHash.insert(iterHash.key(), iterHash.value());
            }
            iterHash.toFront();
        }
    } else{
        while(iterHash.hasNext()){
            iterHash.next();
            for(int i = 0; i != stlSelectedItems.size(); ++i){
                if( iterHash.key() == stlSelectedItems.at(i) )
                    tmpHash.insert(iterHash.key(), iterHash.value());
            }
        }
    }

    if (retSum)
        return getSizeOfFiles(tmpHash);
    else
        return tmpHash.size();
}


//
void ZBC_SideFrame::setTextForLblDirInfo(QLabel * plbl)
{
    setListOfItemsInDir();
    plbl->setText(QString::number(getSizeOfSelectedFiles())
                         + " k/ "
                         + QString::number(getSizeOfFiles(m_hashFiles))
                         + " k in "
                         + QString::number(getSizeOfSelectedFiles(false))
                         + "/"
                         + QString::number(m_hashFiles.size())
                         + " files, "
                         + QString::number(QSet<QString>::fromList(stlSelectedItems).intersect(*m_psetDirs).size())
                         + "/"
                         + QString::number(m_psetDirs->size())
                         + " dirs");

    m_hashFiles.clear();
    m_psetDirs->clear();
}





















