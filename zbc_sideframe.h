#ifndef ZBCSIDEFRAME_H
#define ZBCSIDEFRAME_H

#include <QFrame>
#include <QSet>

class QLabel;

class ZBC_SideFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ZBC_SideFrame(const QString path, QWidget *pwgt = 0);
    ~ZBC_SideFrame();

    void clearListOfSelectedItems();
    QString getCurrentPath();
    QStringList getListOfSelectedItems() const;

private:
    QString                     m_sCurPath;
    QStringList                 stlSelectedItems;

    QHash<QString, int>         m_hashFiles;
    QSet<QString>               m_setDirs;

    void setListOfItemsInDir();
    qint64 getSizeOfFiles(QHash<QString, int>) const;
    qint64 getSizeOfSelectedFiles( bool retSum = true );
    void setTextForLblDirInfo(QLabel*);

signals:
    void Active();
};

#endif // ZBCSIDEFRAME_H

