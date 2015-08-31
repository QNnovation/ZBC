#ifndef ZBCSIDEFRAME_H
#define ZBCSIDEFRAME_H

#include <QFrame>
#include <QModelIndex>

class QComboBox;
class QFileSystemModel;
class QFileSystemWatcher;
class QLabel;
class QSortFilterProxyModel;
class QTreeView;

class zbcSideFrame : public QFrame
{
    Q_OBJECT

public:
    explicit zbcSideFrame(QWidget *pwgt = 0);
    ~zbcSideFrame();

    QStringList getListOfSelectedItems() const;
    void clearListOfSelectedItems();
    QString getCurrentPath() const;
    const QFileSystemModel* getModel()const;

private:
//MEMBERS
    QFileSystemModel*           m_pfsmModel;
    QFileSystemWatcher*         m_pfsWatcher;
    QSortFilterProxyModel*      m_psfpModel;
    QTreeView*                  m_ptblView;
    QComboBox*                  m_pcbxVolumes;
    QLabel*                     m_plblCurPath;
    QString*                    m_pstrCurPath;
    QString*                    m_pstrCurDisk;
    QString*                    m_pstrCurDiskLabel;
    QStringList*                m_pstlSelectedItems;

//METHODS
    void createView();
    void createConnections();
    void getStringOfDrives(const QString&);

private slots:
    void doubleClickTableViewItem(QModelIndex _index);
    void getSelectedItems();
    void setCurrentIndexFromString(QString);
    void TESTWatcherSendSignal(QString);

public slots:
    void makeDir(QString*);
    void setRootPathForFS(QString);

signals:
    void Active(zbcSideFrame*);
    void dirCreated();
    void dirNotCreated();
};

#endif // ZBCSIDEFRAME_H
