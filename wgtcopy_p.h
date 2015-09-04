#ifndef WGTCOPY_P
#define WGTCOPY_P
#include "wgtcopy.h"

class FileOperationsPrivate
{
    Q_DECLARE_PUBLIC(FileOperations);

public:
    FileOperationsPrivate();
    virtual ~FileOperationsPrivate();

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
}

#endif // WGTCOPY_P

