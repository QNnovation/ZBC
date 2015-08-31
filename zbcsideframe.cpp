#include <QDebug>

#include "zbcsideframe.h"

#include <QComboBox>
#include <QDirModel>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QGridLayout>
#include <QLabel>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QUrl>

//C-tor
zbcSideFrame::zbcSideFrame(QWidget *pwgt) : QFrame(pwgt)
{
    createView();
    createConnections();
}


//D-tor
zbcSideFrame::~zbcSideFrame(){}


//Create and layout objects
void zbcSideFrame::createView()
{
//Model
    m_pfsmModel             = new QFileSystemModel(this);
    m_pfsmModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
    m_pfsmModel->setRootPath("");
    m_pfsmModel->setReadOnly(false);

//Watcher
    m_pfsWatcher            = new QFileSystemWatcher;

//TreeView
    m_ptblView              = new QTreeView(this);
    m_ptblView->setRootIsDecorated(false);
    m_ptblView->setItemsExpandable(false);
    m_ptblView->setModel(m_pfsmModel);
    m_ptblView->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    m_ptblView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_ptblView->setEditTriggers(QTreeView::NoEditTriggers);


//ComboBox as View
    m_pcbxVolumes           = new QComboBox(this);
    m_pcbxVolumes->setModel(m_pfsmModel);
    m_pcbxVolumes->setMaximumSize(m_pcbxVolumes->sizeHint());
//    m_pcbxVolumes->setMinimumSize( m_pcbxVolumes->sizeHint() );

//Current path
    m_pstrCurPath           = new QString;
    *m_pstrCurPath = m_pfsmModel->rootPath();
    m_pstrCurDisk           = new QString;
    m_pstrCurDiskLabel      = new QString;

//Label with current path
    m_plblCurPath           = new QLabel(this);
    m_plblCurPath->setText(this->getCurrentPath());

    //List of Selected Items;
m_pstlSelectedItems     = new QStringList;

//Layout
    QGridLayout* pgrdLayout = new QGridLayout;
    pgrdLayout->setMargin(0);
    pgrdLayout->addWidget(m_pcbxVolumes, 0, 0, 1, 2);
    pgrdLayout->addWidget(m_plblCurPath, 1, 0, 1, 10);
    pgrdLayout->addWidget(m_ptblView, 2, 0, 20, 20);

    this->setLayout(pgrdLayout);
}


//Connections
void zbcSideFrame::createConnections()
{
    connect(m_ptblView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickTableViewItem(QModelIndex)));

    connect(m_ptblView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(getSelectedItems()));

    connect(m_pcbxVolumes, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(setCurrentIndexFromString(QString)));
}


//Return list of selected files
QStringList zbcSideFrame::getListOfSelectedItems() const
{
    return *m_pstlSelectedItems;
}


//Clear QStringList with Selected Items
void zbcSideFrame::clearListOfSelectedItems()
{
    m_pstlSelectedItems->clear();
}


//Return current path
QString zbcSideFrame::getCurrentPath() const
{
    if(m_pstrCurPath->length() == 3)
        return *m_pstrCurPath;
    return (*m_pstrCurPath + "/");
}


/*Not ready*/
const QFileSystemModel* zbcSideFrame::getModel() const
{
    return m_pfsmModel;
}


//get drive and its label from QString
void zbcSideFrame::getStringOfDrives(const QString& _sPath)
{
    if(_sPath.length() == 2)
    {
        *m_pstrCurDisk = _sPath;
        *m_pstrCurDiskLabel = "_none_";
    }
    else
    {
        *m_pstrCurDisk = _sPath.right(3);
        m_pstrCurDisk->remove(')');
    }
}


//Double Click Handler for zbcTableView
void zbcSideFrame::doubleClickTableViewItem(QModelIndex _index)
{
    QString         sPath(m_pfsmModel->filePath(_index));
    QModelIndex     index(m_pfsmModel->index(sPath));

    if(QFileInfo(m_pfsmModel->filePath(index)).isDir())
    {
        m_ptblView->setRootIndex(index);
        m_pstlSelectedItems->clear();
        sPath = m_pfsmModel->filePath(index);
        *m_pstrCurPath = sPath;
        m_plblCurPath->setText(this->getCurrentPath());
    }
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(sPath));
}


//Get path string from QComboBox and set this at QTableView
void zbcSideFrame::setCurrentIndexFromString(QString _sPath)
{
    getStringOfDrives(_sPath);
    *m_pstrCurPath = *m_pstrCurDisk;
    m_plblCurPath->setText(this->getCurrentPath());
    m_ptblView->setRootIndex(m_pfsmModel->index(*m_pstrCurDisk));
}


//Get Selected Items
void zbcSideFrame::getSelectedItems()
{
    QModelIndexList* plstIndexes = new QModelIndexList;
    *plstIndexes = m_ptblView->selectionModel()->selectedRows();

    m_pstlSelectedItems->clear();
    foreach (QModelIndex index, *plstIndexes)
    {
        if( (m_pfsmModel->filePath(index).right(2)) == ".." )
                continue;
        if(m_pfsmModel->isDir(index))
            m_pstlSelectedItems->push_back(m_pfsmModel->filePath(index) + "/");
        else
            m_pstlSelectedItems->push_back(m_pfsmModel->filePath(index));
    }
    plstIndexes->clear();

    emit Active(this);
}


//Make new folder
void zbcSideFrame::makeDir(QString * _pstrName)
{
    if( !QDir(*m_pstrCurPath + "/" + *_pstrName).exists() )
    {
        m_pfsmModel->mkdir(m_pfsmModel->index(*m_pstrCurPath), *_pstrName);
        emit dirCreated();
    }
    else
        emit dirNotCreated();
}


//Set root path for QFileSystemModel. Use for update info about file size
//***Does not work!!!
void zbcSideFrame::setRootPathForFS(QString _str)
{
qDebug() << "Not Updated";

qDebug() << _str;

    m_pfsmModel->setRootPath("");
    m_pfsmModel->setRootPath(*m_pstrCurPath);

qDebug() << "Updated";
}


void zbcSideFrame::TESTWatcherSendSignal(QString _str)
{
qDebug() << _str;
}
