#ifndef ZBCSIDEFRAME_H
#define ZBCSIDEFRAME_H

#include <QFrame>
#include <QModelIndex>

class QFileSystemModel;
class QLabel;
class QTreeView;

class zbcSideFrame : public QFrame
{
    Q_OBJECT

public:
    explicit zbcSideFrame(QWidget *pwgt = 0);
    ~zbcSideFrame();

    QStringList getListOfSelectedItems() const;
    void clearListOfSelectedItems();
    QString getCurrentPath();
    const QFileSystemModel* getModel()const;

private:
//MEMBERS
    QFileSystemModel*           m_pfsmModel;
    QTreeView*                  m_ptblView;
    QLabel*                     m_plblCurPath;
    QString                     sCurPath;
    QString                     sCurDisk;
    QString                     sCurDiskLabel;
    QStringList                 stlSelectedItems;

//METHODS
    void getStringOfDrives(const QString&);

private slots:
    void doubleClickTableViewItem(QModelIndex _index);
    void getSelectedItems();
    void setCurrentIndexFromString(QString);

public slots:
    void makeDir(QString*);
    void setRootPathForFS(QString);

signals:
    void Active(zbcSideFrame*);
    void dirCreated();
    void dirNotCreated();
};

#endif // ZBCSIDEFRAME_H
