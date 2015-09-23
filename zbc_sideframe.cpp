#include "zbc_sideframe.h"
#include "zbc_treeview.h"

#include <QComboBox>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QModelIndex>
#include <QUrl>


//C-tor
ZBC_SideFrame::ZBC_SideFrame(const QString path, QWidget *pwgt) : QFrame(pwgt)
{
//Model
    QFileSystemModel* pfsmModel             = new QFileSystemModel(this);
    pfsmModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
    pfsmModel->setRootPath(path);
    pfsmModel->setReadOnly(false);

//TreeView
    ZBC_TreeView* ptreeView         = new ZBC_TreeView(this);
    ptreeView->setModel(pfsmModel);
    ptreeView->setRootIndex(pfsmModel->index(path));

//ComboBox as View
    QComboBox*  pcbxVolumes     = new QComboBox(this);
    pcbxVolumes->setModel(pfsmModel);
    pcbxVolumes->setMaximumSize(pcbxVolumes->sizeHint());

//Current path
    m_sCurPath = pfsmModel->rootPath();

//Label with current path
    QLabel* plblCurPath           = new QLabel(this);
    plblCurPath->setText(this->getCurrentPath());

//Layout
    QGridLayout* pgrdLayout = new QGridLayout;
    pgrdLayout->setMargin(0);
    pgrdLayout->addWidget(pcbxVolumes, 0, 0, 1, 2);
    pgrdLayout->addWidget(plblCurPath, 1, 0, 1, 10);
    pgrdLayout->addWidget(ptreeView, 2, 0, 20, 20);

    this->setLayout(pgrdLayout);

//Connections
//Change value of QComboBox with drives
    connect(pcbxVolumes,
            static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            [this, pfsmModel, ptreeView, plblCurPath](QString _sPath){
                QString sCurDisk;
                if (_sPath.length() == 2)
                    sCurDisk = _sPath;
                else{
                    sCurDisk = _sPath.right(3);
                    sCurDisk.remove(')');
                }

                m_sCurPath = sCurDisk;
                plblCurPath->setText(getCurrentPath());
                ptreeView->setRootIndex(pfsmModel->index(sCurDisk));}
            );

//Double click on item at QTreeView(change dir or open file)
    connect(ptreeView,
            &QTreeView::doubleClicked,
            [this, pfsmModel, ptreeView, plblCurPath](const QModelIndex &_index){
                if (QFileInfo(pfsmModel->filePath(_index)).isDir()){

                    if ( (pfsmModel->filePath(_index)) == ".." )
                        ptreeView->setRootIndex(QModelIndex(pfsmModel->index("..")));
                    else
                        ptreeView->setRootIndex(QModelIndex(pfsmModel->index(pfsmModel->filePath(_index))));

                    m_sCurPath = pfsmModel->filePath(ptreeView->rootIndex());
                    plblCurPath->setText(getCurrentPath());
                    stlSelectedItems.clear();
                }
                else
                QDesktopServices::openUrl(QUrl::fromLocalFile(pfsmModel->filePath(_index)));}
            );

//Fill QStringList with selected items
    connect(ptreeView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            [this, pfsmModel, ptreeView](){
                QModelIndexList* plstIndexes = new QModelIndexList;
                *plstIndexes = ptreeView->selectionModel()->selectedRows();

                stlSelectedItems.clear();
                foreach (QModelIndex index, *plstIndexes){
                    if ( (pfsmModel->filePath(index).right(2)) == ".." )
                        continue;
                    if (pfsmModel->isDir(index))
                        stlSelectedItems.push_back(pfsmModel->filePath(index) + "/");
                    else
                        stlSelectedItems.push_back(pfsmModel->filePath(index));
                }
                plstIndexes->clear();
            });

//Send Signal about active TreeView
    connect(ptreeView,
            &ZBC_TreeView::Active,
            this,
            &ZBC_SideFrame::Active);
}


//D-tor
ZBC_SideFrame::~ZBC_SideFrame(){}


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
