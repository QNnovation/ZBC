#include <QDebug>

#include "zbc_drivebuttonswidget.h"
#include "zbc_sideframe.h"
#include "zbc_lineedit.h"
#include "zbc_treeview.h"

#include <QComboBox>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QModelIndex>
#include <QPushButton>
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

//Buttons with Drives
    QStringList     lstDrives;
    for( QFileInfo file : QDir::drives() ){
        lstDrives.push_back(file.filePath().left(2) + QDir::separator());
    }
    ZBC_DriveButtonsWidget* pdrvButtons    = new ZBC_DriveButtonsWidget(lstDrives, this);

//ComboBox as View
    QComboBox*  pcbxVolumes         = new QComboBox(this);
    pcbxVolumes->setFocusPolicy(Qt::NoFocus);
    for (QString sDrive : lstDrives)
        pcbxVolumes->addItem(QIcon(":/buttons/drives/resource/logicaldrive.ico"), sDrive);
    pcbxVolumes->setCurrentIndex(lstDrives.indexOf(path.left(3)));
    pcbxVolumes->setMaximumSize(pcbxVolumes->sizeHint());

//Current path
    m_sCurPath = QDir::toNativeSeparators(pfsmModel->rootPath());
    m_lstPathHistory.push_front(m_sCurPath);
    m_iterPathHistory = m_lstPathHistory.constBegin();
    qDebug() << "CTR: " << m_lstPathHistory;
    qDebug() << *m_iterPathHistory;

//LineEdit with current path
    ZBC_LineEdit* pledCurPath      = new ZBC_LineEdit(this);
    pledCurPath->setReadOnly(true);
    pledCurPath->setFocusPolicy(Qt::NoFocus);
    pledCurPath->setText(this->m_sCurPath);

//Label with info about files and dirs
    QLabel* plblDirInfo         = new QLabel(this);
    plblDirInfo->setFocusPolicy(Qt::NoFocus);
    plblDirInfo->setFrameStyle(QFrame::Panel | QFrame::Plain);
    setListOfItemsInDir();
    setTextForLblDirInfo(plblDirInfo);

//Layout
    QGridLayout* pgrdLayout = new QGridLayout(this);
    pgrdLayout->setMargin(5);
    pgrdLayout->addWidget(pcbxVolumes, 0, 0);
    pgrdLayout->addWidget(pdrvButtons, 0, 1);
    pgrdLayout->addWidget(pledCurPath,1, 0, 1, 20);
    pgrdLayout->addWidget(ptreeView, 2, 0, 20, 20);
    pgrdLayout->addWidget(plblDirInfo, 22, 0, 1, 20);
    this->setLayout(pgrdLayout);

//Style
    this->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

//Connections
//Change value of QComboBox with drives
    connect(pcbxVolumes,
            static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            [=](QString _sPath){
                QString sCurDisk;
                if (_sPath.length() == 2)
                    sCurDisk = _sPath;
                else{
                    sCurDisk = _sPath.right(3);
                    sCurDisk.remove(')');
                }
                m_sCurPath = sCurDisk;
                m_lstPathHistory.push_front(m_sCurPath);
                m_iterPathHistory = m_lstPathHistory.constBegin();
                qDebug() << "CBX: " << m_lstPathHistory;
                qDebug() << *m_iterPathHistory;

                pledCurPath->setText(m_sCurPath);
                ptreeView->setRootIndex(psfpModel->mapFromSource(pfsmModel->index(sCurDisk)));
                setTextForLblDirInfo(plblDirInfo);
                emit Active();
            });

//Drive buttons clicked
    connect(pdrvButtons,
            &ZBC_DriveButtonsWidget::clicked,
            [=](QString sDrvPath){
                m_sCurPath = sDrvPath;
                m_lstPathHistory.push_front(m_sCurPath);
                m_iterPathHistory = m_lstPathHistory.constBegin();
                qDebug() << "DRV: " << m_lstPathHistory;
                qDebug() << *m_iterPathHistory;

                pledCurPath->setText(m_sCurPath);
                pcbxVolumes->setCurrentIndex(lstDrives.indexOf(sDrvPath));
                ptreeView->setRootIndex(psfpModel->mapFromSource(pfsmModel->index(pledCurPath->text())));
                setTextForLblDirInfo(plblDirInfo);
            });

//Enter pressed on LineEdit with current path
    connect(pledCurPath,
            &ZBC_LineEdit::pressedEnter,
            [=](QString oldVal){
                QDir tmpDir(pledCurPath->text());
                if (!tmpDir.exists())
                    pledCurPath->setText(oldVal);
                else
                    ptreeView->setRootIndex(QModelIndex( psfpModel->mapFromSource(
                                                             pfsmModel->index(pledCurPath->text()))));
                m_sCurPath = QDir::toNativeSeparators( pfsmModel->filePath(
                                                           psfpModel->mapToSource(ptreeView->rootIndex())) );
                m_lstPathHistory.push_front(m_sCurPath);
                m_iterPathHistory = m_lstPathHistory.constBegin();
                qDebug() << "LED: " << m_lstPathHistory;
                qDebug() << *m_iterPathHistory;


                stlSelectedItems.clear();
                setTextForLblDirInfo(plblDirInfo);
                emit Active();
            });

//Double click on item at QTreeView(change dir or open file)
    connect(ptreeView,
            &QTreeView::doubleClicked,
            [=](const QModelIndex &_index){
                if (QFileInfo(pfsmModel->filePath(psfpModel->mapToSource(_index))).isDir()){
                    if ( (pfsmModel->permissions(psfpModel->mapToSource(_index))) == (QFlags<QFile::Permissions>(0)) )
                        QMessageBox::critical(this,
                                              tr("ZBC Error"),
                                              tr("Access denied on folder %1").arg(pfsmModel->filePath(_index)),
                                              QMessageBox::Ok);
                    else{
                        if ( (pfsmModel->filePath(psfpModel->mapToSource(_index))) == ".." )
                            ptreeView->setRootIndex(QModelIndex( psfpModel->mapFromSource(pfsmModel->index(".."))));
                        else{
                            ptreeView->setRootIndex(psfpModel->mapFromSource(pfsmModel->index(pfsmModel->filePath(
                                                                                                  psfpModel->mapToSource(_index)))));
                        }
                        m_sCurPath = QDir::toNativeSeparators( pfsmModel->filePath( psfpModel->mapToSource(ptreeView->rootIndex())) );
                        m_lstPathHistory.push_front(m_sCurPath);
                        m_iterPathHistory = m_lstPathHistory.constBegin();
                        qDebug() << "TRV: " << m_lstPathHistory;
                        qDebug() << *m_iterPathHistory;

                        pledCurPath->setText(m_sCurPath);
                        stlSelectedItems.clear();
                        setTextForLblDirInfo(plblDirInfo);
                    }
                }
                else
                    QDesktopServices::openUrl(QUrl::fromLocalFile(pfsmModel->filePath(psfpModel->mapToSource(_index))));
                emit Active();
            });

//Fill QStringList with selected items
    connect(ptreeView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            [=](){
                stlSelectedItems.clear();
                for ( const QModelIndex& index:ptreeView->selectionModel()->selectedRows()){
                    if ( (pfsmModel->filePath(psfpModel->mapToSource(index)).right(2)) == QLatin1String("..") )
                        continue;
                    if ( pfsmModel->isDir(psfpModel->mapToSource(index)) )
                        stlSelectedItems.push_back(QDir::toNativeSeparators(pfsmModel->filePath(
                                                                                psfpModel->mapToSource(index))) + QDir::separator());
                    else
                        stlSelectedItems.push_back(QDir::toNativeSeparators(pfsmModel->filePath(psfpModel->mapToSource(index))));
                }
                setTextForLblDirInfo(plblDirInfo);
            });

//Send Signal about active TreeView
    connect(ptreeView,
            &ZBC_TreeView::Active,
            this,
            &ZBC_SideFrame::Active);

//Signal from MainWindow
//Go Back
    connect(this,
            &ZBC_SideFrame::goBack,
            [=](){
                if ( m_iterPathHistory != m_lstPathHistory.constEnd() && ++m_iterPathHistory != m_lstPathHistory.constEnd() ){
                    ptreeView->setRootIndex(QModelIndex( psfpModel->mapFromSource(pfsmModel->index(*m_iterPathHistory))));
                    m_sCurPath = QDir::toNativeSeparators( pfsmModel->filePath( psfpModel->mapToSource(ptreeView->rootIndex())) );
                    pledCurPath->setText(m_sCurPath);
                    stlSelectedItems.clear();
                    setTextForLblDirInfo(plblDirInfo);
                    qDebug() << "Back" << m_lstPathHistory;
                    qDebug() << *m_iterPathHistory;
                }
            });

//Go Forward
    connect(this,
            &ZBC_SideFrame::goForward,
            [=](){
                if( m_iterPathHistory != m_lstPathHistory.constBegin()){
                    ptreeView->setRootIndex(QModelIndex( psfpModel->mapFromSource(pfsmModel->index(*--m_iterPathHistory))));
                    m_sCurPath = QDir::toNativeSeparators( pfsmModel->filePath( psfpModel->mapToSource(ptreeView->rootIndex())) );
                    pledCurPath->setText(m_sCurPath);
                    stlSelectedItems.clear();
                    setTextForLblDirInfo(plblDirInfo);
                    qDebug() << "Forward" << m_lstPathHistory;
                    qDebug() << *m_iterPathHistory;
                }
            });
}


//D-tor
ZBC_SideFrame::~ZBC_SideFrame()
{}


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
    const int ROOTDRIVELENGTH = 4;
    if (m_sCurPath.length() > ROOTDRIVELENGTH )
        return  QDir::toNativeSeparators(m_sCurPath) + QDir::separator();
    return  QDir::toNativeSeparators(m_sCurPath);
}


//Number of files and dors in folder
void ZBC_SideFrame::setListOfItemsInDir()
{
    static const int KBytes = 1024;

    QDirIterator* pdirIter      = new QDirIterator(m_sCurPath,
                                                   QDir::Files
                                                   | QDir::NoSymLinks
                                                   | QDir::NoDotAndDotDot);
    while (pdirIter->hasNext()){
        pdirIter->next();
        m_hashFiles.insert( QDir::toNativeSeparators(pdirIter->filePath()), pdirIter->fileInfo().size() / KBytes);
    }
    delete pdirIter;

    pdirIter                    = new QDirIterator(m_sCurPath,
                                                   QDir::Dirs
                                                   | QDir::NoSymLinks
                                                   | QDir::NoDotAndDotDot);
    while (pdirIter->hasNext())
        m_setDirs.insert( QDir::toNativeSeparators(pdirIter->next()) + QDir::separator());
    delete pdirIter;
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
                         + QLatin1String(" k/ ")
                         + QString::number(getSizeOfFiles(m_hashFiles))
                         + QLatin1String(" k in ")
                         + QString::number(getSizeOfSelectedFiles(false))
                         + QLatin1String("/")
                         + QString::number(m_hashFiles.size())
                         + QLatin1String(" files, ")
                         + QString::number(QSet<QString>::fromList(stlSelectedItems).intersect(m_setDirs).size())
                         + QLatin1String("/")
                         + QString::number(m_setDirs.size())
                         + QLatin1String(" dirs"));

    m_hashFiles.clear();
    m_setDirs.clear();
}
