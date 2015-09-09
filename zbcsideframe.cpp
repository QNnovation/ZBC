#include <QDebug>

#include "zbcsideframe.h"

#include <QComboBox>
#include <QDirModel>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QModelIndex>
#include <QTreeView>
#include <QUrl>


//C-tor
zbcSideFrame::zbcSideFrame(QWidget *pwgt) : QFrame(pwgt)
{
//Model
    m_pfsmModel             = new QFileSystemModel(this);
    m_pfsmModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
    m_pfsmModel->setRootPath("");
    m_pfsmModel->setReadOnly(false);

//TreeView
    m_ptblView              = new QTreeView(this);
    m_ptblView->setRootIsDecorated(false);
    m_ptblView->setItemsExpandable(false);
    m_ptblView->setModel(m_pfsmModel);
    m_ptblView->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    m_ptblView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_ptblView->setEditTriggers(QTreeView::NoEditTriggers);

//ComboBox as View
    QComboBox*  pcbxVolumes     = new QComboBox(this);
    pcbxVolumes->setModel(m_pfsmModel);
    pcbxVolumes->setMaximumSize(pcbxVolumes->sizeHint());

//Current path
    sCurPath = m_pfsmModel->rootPath();

//Label with current path
    m_plblCurPath           = new QLabel(this);
    m_plblCurPath->setText(this->getCurrentPath());


//Layout
    QGridLayout* pgrdLayout = new QGridLayout;
    pgrdLayout->setMargin(0);
    pgrdLayout->addWidget(pcbxVolumes, 0, 0, 1, 2);
    pgrdLayout->addWidget(m_plblCurPath, 1, 0, 1, 10);
    pgrdLayout->addWidget(m_ptblView, 2, 0, 20, 20);

    this->setLayout(pgrdLayout);

//Connections
//Is it better than is was? (sender, SIGNAL(), reciver, SLOT())
//I think it's ugly
    connect(pcbxVolumes,
            static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this,
            &zbcSideFrame::setCurrentIndexFromString);

    connect(m_ptblView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClickTableViewItem(QModelIndex)));

    connect(m_ptblView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(getSelectedItems()));
}


//D-tor
zbcSideFrame::~zbcSideFrame(){}


//Return list of selected files
QStringList zbcSideFrame::getListOfSelectedItems() const
{
    return stlSelectedItems;
}


//Clear QStringList with Selected Items
void zbcSideFrame::clearListOfSelectedItems()
{
    stlSelectedItems.clear();
}


//Return current path
QString zbcSideFrame::getCurrentPath()
{
    if(sCurPath.length() == 3)
        return sCurPath;
    return (sCurPath + "/");
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
        sCurDisk = _sPath;
        sCurDiskLabel = "_none_";
    }
    else
    {
        sCurDisk = _sPath.right(3);
        sCurDisk.remove(')');
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
        stlSelectedItems.clear();
        sPath = m_pfsmModel->filePath(index);
        sCurPath = sPath;
        m_plblCurPath->setText(this->getCurrentPath());
    }
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(sPath));
}


//Get path string from QComboBox and set this at QTableView
void zbcSideFrame::setCurrentIndexFromString(QString _sPath)
{
    getStringOfDrives(_sPath);
    sCurPath = sCurDisk;
    m_plblCurPath->setText(this->getCurrentPath());
    m_ptblView->setRootIndex(m_pfsmModel->index(sCurDisk));
}


//Get Selected Items
void zbcSideFrame::getSelectedItems()
{
    QModelIndexList* plstIndexes = new QModelIndexList;
    *plstIndexes = m_ptblView->selectionModel()->selectedRows();

    stlSelectedItems.clear();
    foreach (QModelIndex index, *plstIndexes)
    {
        if( (m_pfsmModel->filePath(index).right(2)) == ".." )
                continue;
        if(m_pfsmModel->isDir(index))
            stlSelectedItems.push_back(m_pfsmModel->filePath(index) + "/");
        else
            stlSelectedItems.push_back(m_pfsmModel->filePath(index));
    }
    plstIndexes->clear();

    emit Active(this);
}


//Make new folder
void zbcSideFrame::makeDir(QString * _pstrName)
{
    if( !QDir(sCurPath + "/" + *_pstrName).exists() )
    {
        m_pfsmModel->mkdir(m_pfsmModel->index(sCurPath), *_pstrName);
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
    m_pfsmModel->setRootPath(sCurPath);

qDebug() << "Updated";
}
