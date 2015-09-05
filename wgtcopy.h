#ifndef WGTCOPY_H
#define WGTCOPY_H

#include <QDialog>
#include <QObject>
#include <QFile>
#include <QMutex>
#include <QWaitCondition>
class QThread;
class QProgressBar;
class QPushButton;
class QLabel;
class QGridLayout;
class QHBoxLayout;
class QDir;

class FileOperationsPrivate;
class FileOperations : public QObject
{
    Q_OBJECT

public:
    explicit FileOperations(QObject *parent = 0);
    void loadFiles(QStringList files_Patch, const QString files_Destination, char act = 'c');
    void copyDirs(const QDir& dir);
    void remDirsFiles(const QStringList&);

    //variables for thread control
    bool thread_Pause;
    bool thread_Break;
    //thread control
    bool Resume();
    bool Pause();
    bool Stop();
    bool spaceCheck(QString path, quint64 size);
    int numFiles();
    ~FileOperations();

public slots:
    void process();

signals:
    void copyStatus(int);
    void copyFilesStatus(int);
    void currentFile(QString);
    void currentPath(QString);
    void copySpeed(QString);
    void formClose(bool);
    void copiedFiles(QString);
    void copiedSize(QString);
    void copiedTime(QString);
    void finished();

protected:
    FileOperationsPrivate *const d_ptr;
    //QScopedPointer<FileOperationsPrivate> const d_ptr;
    FileOperations(FileOperationsPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(FileOperations);

};

//GUI window
class FileOperationWgt : public QDialog
{
    Q_OBJECT

public:
    explicit FileOperationWgt(QWidget *parent = 0);

    QProgressBar* fileCopyStatus;
    QProgressBar* filesCopyStatus;
    QPushButton* pauseStartBtn;
    QPushButton* backgroundBtn;
    QPushButton* quitBtn;
    QLabel* copyStatusSpeed;
    QLabel* numberOfFiles;
    QLabel* copyFrom;
    QLabel* copyTo;
    QLabel* sizeOfFiles;

    QGridLayout* mainLayout;
    QHBoxLayout* hLayout;

    void copyFileOperation(const QStringList&, const QString&);
    void moveFileOperation(const QStringList&, const QString&);
    void removeFileOperation(const QStringList&);

    bool confirmOperation();

    ~FileOperationWgt();

public slots:
    void toBackground();
    void threadPauseResume();
    void closeWgt();
    //Toxa
    void currentFileName(QString);

private:
    QStringList inputFiles;
    QThread* thread;
    FileOperations* fileoperations;

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void finishOperation();
    //Toxa
    void curFile(QString);

};

#endif // WGTCOPY_H
