#ifndef WGTCOPY_P
#define WGTCOPY_P
#include "wgtcopy.h"

class QThread;
class QProgressBar;
class QPushButton;
class QLabel;
class QGridLayout;
class QHBoxLayout;

class FileOperationsPrivate
{
    Q_DECLARE_PUBLIC(FileOperations);

public:
    FileOperationsPrivate();
    ~FileOperationsPrivate();

    bool b_copyFileOperation;
    bool b_moveFileOperation;
    bool b_removeFileOperation;

    QFile inputFile;
    QFile outputFile;

    QString fileDestination;

    QStringList filesDestination;
    QStringList filesPath;
    QStringList deleteFileList;
    QStringList deleteDirList;

    QMutex sync;
    QWaitCondition pauseCond;

    quint64 filesSize;
    FileOperations *q_ptr;

};

class FileOperationWgtPrivate
{
  Q_DECLARE_PUBLIC(FileOperationWgt);

public:
    FileOperationWgtPrivate();
    ~FileOperationWgtPrivate();

    QProgressBar *fileCopyStatus;
    QProgressBar *filesCopyStatus;
    QPushButton *pauseStartBtn;
    QPushButton *backgroundBtn;
    QPushButton *quitBtn;
    QLabel *copyStatusSpeed;
    QLabel *numberOfFiles;
    QLabel *copyFrom;
    QLabel *copyTo;
    QLabel *sizeOfFiles;

    QGridLayout *mainLayout;
    QHBoxLayout *hLayout;

    QStringList inputFiles;
    QThread *thread;
    FileOperations *fileoperations;

    FileOperationWgt *q_ptr;
};

#endif // WGTCOPY_P

