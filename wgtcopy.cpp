/*
 * Created by Lysytskyi Oleksii 2015
*/

#include "wgtcopy.h"
#include "wgtcopy_p.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QMessageBox>
#include <QDirIterator>
#include <QThread>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <windows.h>

quint64 const gigaByte = 1073741824;
quint64 const megaByte = 1048576;
quint32 const kiloByte = 1024;

//FileOperationWgt class implementation***/
FileOperationWgt::FileOperationWgt(QWidget *parent)
    : QDialog(parent)
    ,d_ptr(new FileOperationWgtPrivate())
{
    Q_D(FileOperationWgt);
    d->q_ptr = this;

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    setWindowTitle("ZBC Copying");
    setFixedSize(450, 128); //125 175

    setLayout(d->mainLayout);

    connect(d->quitBtn, SIGNAL(clicked()), this, SLOT(closeWgt()));
    connect(d->pauseStartBtn, SIGNAL(clicked()), this, SLOT(threadPauseResume()));
    connect(d->backgroundBtn, SIGNAL(clicked()), this, SLOT(toBackground()));
    connect(d->fileoperations, SIGNAL(copyStatus(int)), d->fileCopyStatus, SLOT(setValue(int)));
    connect(d->fileoperations, SIGNAL(copyFilesStatus(int)), d->filesCopyStatus, SLOT(setValue(int)));
    connect(d->fileoperations, SIGNAL(copySpeed(QString)), d->copyStatusSpeed, SLOT(setText(QString)));
    connect(d->fileoperations, SIGNAL(currentFile(QString)), d->copyFrom, SLOT(setText(QString)));
    connect(d->fileoperations, SIGNAL(currentPath(QString)), d->copyTo, SLOT(setText(QString)));
    connect(d->fileoperations, SIGNAL(copiedFiles(QString)), d->numberOfFiles, SLOT(setText(QString)));
    connect(d->fileoperations, SIGNAL(copiedSize(QString)), d->sizeOfFiles, SLOT(setText(QString)));

    //connect signals from copy to gui
    connect(d->fileoperations, SIGNAL(finished()), this, SIGNAL(finishOperation()));

    //mtoxa
    connect(d->fileoperations, SIGNAL(currentPath(QString)), this, SLOT(currentFileName(QString)));

    //connect thread start signal
    connect(d->thread, SIGNAL(started()), d->fileoperations, SLOT(process()));
    //Right thread exit
    connect(d->fileoperations, SIGNAL(finished()), d->thread, SLOT(quit()));
    connect(d->thread, SIGNAL(finished()), d->thread, SLOT(deleteLater()));
    //close widget window
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(d->fileoperations, SIGNAL(formClose(bool)), this, SLOT(closeWgt()));
}

//copy files
void FileOperationWgt::copyFileOperation(const QStringList& files, const QString& path)
{
    Q_D(const FileOperationWgt);
    if (confirmOperation())
    {
        d->fileoperations->loadFiles(files, path, 'c');
        if (d->fileoperations->numFiles() >  1)
        {
            setFixedSize(450, 175);
            d->filesCopyStatus->setVisible(true);
            d->sizeOfFiles->setVisible(true);
            d->numberOfFiles->setVisible(true);
        }
        d->thread->start();
    }
}

//move files
void FileOperationWgt::moveFileOperation(const QStringList& files, const QString& path)
{
    Q_D(const FileOperationWgt);
    if (confirmOperation())
    {
        d->fileoperations->loadFiles(files, path, 'm');
        if (d->fileoperations->numFiles() >  1)
        {
            setFixedSize(450, 175);
            d->filesCopyStatus->setVisible(true);
            d->sizeOfFiles->setVisible(true);
            d->numberOfFiles->setVisible(true);
        }
        d->thread->start();
    }
}

//remove files
void FileOperationWgt::removeFileOperation(const QStringList& files)
{
    Q_D(const FileOperationWgt);
    if (confirmOperation())
    {
        d->copyStatusSpeed->setText(tr("Deleting"));
        d->copyTo->setVisible(false);
        d->numberOfFiles->setVisible(false);
        d->sizeOfFiles->setVisible(false);
        d->filesCopyStatus->setVisible(false);
        d->fileoperations->remDirsFiles(files);
    }
}

bool FileOperationWgt::confirmOperation()
{
    QMessageBox::StandardButton confirmBox;
    confirmBox = QMessageBox::question(this, "Confirm operation",
                                       "Are you sure ?",
                                       QMessageBox::Ok | QMessageBox::Cancel);
    if (confirmBox == QMessageBox::Ok)
        return true;
    else
        this->close();
    return false;

}

void FileOperationWgt::threadPauseResume()
{
    Q_D(const FileOperationWgt);
    if (!d->fileoperations->thread_Pause)
        d->fileoperations->Pause();
    else
        d->fileoperations->Resume();
}

void FileOperationWgt::closeWgt()
{
    Q_D(const FileOperationWgt);
    d->fileoperations->Stop();
    this->close();
}

void FileOperationWgt::currentFileName(QString filePath)
{
    emit curFile(filePath.remove(0, 4));
}

FileOperationWgt::FileOperationWgt(FileOperationWgtPrivate &dd, QWidget *parent)
    : d_ptr(&dd)
{
    Q_D(FileOperationWgt);
    d->q_ptr = this;
}

void FileOperationWgt::closeEvent(QCloseEvent *event)
{
    Q_D(FileOperationWgt);
    d->fileoperations->Stop();
}

void FileOperationWgt::toBackground()
{
    showMinimized();
}

FileOperationWgt::~FileOperationWgt()
{

}

//FileOperations class implementation
FileOperations::FileOperations(QObject *parent)
    : QObject(parent)
    , d_ptr(new FileOperationsPrivate)
{
    Q_D(FileOperations);
    d->q_ptr = this;
    thread_Pause = false;
    thread_Break = false;
}

bool FileOperations::Resume()
{
    d_ptr->sync.lock();
    thread_Pause = false;
    d_ptr->sync.unlock();
    d_ptr->pauseCond.wakeAll();
    return thread_Pause;
}

bool FileOperations::Pause()
{
    d_ptr->sync.lock();
    thread_Pause = true;
    d_ptr->sync.unlock();
    return thread_Pause;
}

bool FileOperations::Stop()
{
    d_ptr->sync.lock();
    thread_Pause = false;
    d_ptr->sync.unlock();
    d_ptr->pauseCond.wakeAll();
    thread_Break = true;
    return thread_Break;
}

//checking for free space
bool FileOperations::spaceCheck(QString path, quint64 size)
{
    ULARGE_INTEGER  freeSpace, totalSpace, p3;
    LPCTSTR source = (LPCWSTR)path.utf16();
    if (GetDiskFreeSpaceEx(source, &freeSpace, &totalSpace, &p3))
    {
        if (size > freeSpace.QuadPart)
            return false;
        else
            return true;
    }
}

int FileOperations::numFiles()
{
    Q_D(const FileOperations);
    return d->filesPath.size();
}

void FileOperations::copyDirs(const QDir &dir)
{
    // !Folders creation;
    //create root directory;
    if (!QDir().exists(d_ptr->fileDestination + dir.dirName()))
        QDir().mkdir(d_ptr->fileDestination + dir.dirName());

    QStringList fullPath;
    QStringList dirCreate;

    //iterate dirs to stringList;
    QDirIterator it(dir.path(), QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        if (it.next().right(1) != ".")
            fullPath.push_back(it.filePath());
    }
    //Get path to the create dirs
    for (int i = 0; i < fullPath.size(); ++i) {
        if (QFileInfo(fullPath.at(i)).isDir()) {
            fullPath[i].remove(0, QString(dir.path()).size() - QString(dir.dirName()).size());
            dirCreate.push_back(d_ptr->fileDestination + fullPath.at(i));
        }
    }
    //Create folders;
    for (int i = 0; i < dirCreate.size(); ++i) {
        if (!QDir().exists(dirCreate.at(i))) {
            QDir().mkdir(dirCreate.at(i));
        }
    }
    // !Add files;
    for (int i = 0; i < fullPath.size(); ++i) {
        if (QFileInfo(fullPath.at(i)).isFile()) {
            d_ptr->filesPath.append(fullPath.at(i));
            d_ptr->filesDestination.append(d_ptr->fileDestination + fullPath[i].remove(0, QString(dir.path()).size() - QString(dir.dirName()).size()));
        }
    }
}

void FileOperations::loadFiles(QStringList files_Path, const QString files_Destination, char act)
{
    //checking copy file to self
    QString checkPath;
    for (int i = 0; i < files_Path.size(); ++i) {
        checkPath = files_Path.at(i);
        checkPath = checkPath.left(checkPath.lastIndexOf('/')) + "/";
        if (checkPath == files_Destination) {
            QMessageBox::warning(0, "Abort", "You can not copy a file to itself!");
            files_Path.clear();
        }
    }
    //check flag
    switch(act)
    {
    case 'c':
        d_ptr->b_copyFileOperation = true;
        break;
    case 'm':
        d_ptr->b_moveFileOperation = true;
        break;
    case 'r':
        d_ptr->b_removeFileOperation = true;
        break;
    }

    //check input data, then create path for files
    if (files_Path.isEmpty() && files_Destination.isEmpty())
        return;

    d_ptr->fileDestination = files_Destination;

    for (int i = 0; i < files_Path.size(); ++i) {
        if (QFileInfo(files_Path.at(i)).isFile()) {
            d_ptr->filesPath.push_back(files_Path.at(i));
            d_ptr->filesDestination.push_back(files_Destination + QFileInfo(files_Path.at(i)).fileName());
        } else {
            d_ptr->filesPath.push_back(files_Path.at(i));
            d_ptr->filesDestination.push_back(files_Destination);
            d_ptr->deleteDirList.append(files_Path.at(i));
        }
    }

    //if dir, add files to array
    for (int i = 0; i < d_ptr->filesPath.size(); ++i) {
        if (QFileInfo(d_ptr->filesPath.at(i)).isDir()) {
            copyDirs(QDir(d_ptr->filesPath.at(i)));
        }
    }
    //get the total size of files
    d_ptr->filesSize = 0;
    for (int j = 0; j < d_ptr->filesPath.size(); ++j)
        d_ptr->filesSize += QFileInfo(d_ptr->filesPath.at(j)).size();


    //check for free space
    QString path = d_ptr->fileDestination;
    path = d_ptr->fileDestination.left(2);

    if (!spaceCheck(path, d_ptr->filesSize))
    {
        QMessageBox::warning(0, "Abort", "No more space");
        d_ptr->filesPath.clear();
    }
    //overwrite check
    for (int i = 0; i < d_ptr->filesPath.size(); ++i) {
        QFileInfo info(d_ptr->filesPath.at(i));
        QString checkFile = d_ptr->filesDestination.at(i);
        if (info.isFile()) {
            if (QFile::exists(checkFile)) {
                QMessageBox msgBox;
                msgBox.setText(d_ptr->filesPath.at(i));
                msgBox.setWindowTitle("File exists");
                msgBox.setInformativeText(checkFile);
                msgBox.setIcon(QMessageBox::Question);
                QPushButton *change = msgBox.addButton(tr("Change file"), QMessageBox::ActionRole);
                QPushButton *changeAll = msgBox.addButton(tr("Change all files"), QMessageBox::ActionRole);
                QPushButton *skip = msgBox.addButton(tr("Skip file"), QMessageBox::ActionRole);
                QPushButton *skipAll = msgBox.addButton(tr("Skip all files"), QMessageBox::ActionRole);
                QPushButton *quit = msgBox.addButton(tr("Cancel"), QMessageBox::ActionRole);
                msgBox.setDefaultButton(change);
                msgBox.exec();
                if (msgBox.clickedButton()== change) {

                }
                else if (msgBox.clickedButton()== changeAll) {
                    i = d_ptr->filesPath.size();
                }
                else if (msgBox.clickedButton()== skip) {
                    d_ptr->filesPath.removeAt(i);
                    d_ptr->filesDestination.removeAt(i);
                    i = 0;
                }
                else if (msgBox.clickedButton()== skipAll) {
                    for (int j = i; j < d_ptr->filesPath.size(); ++j) {
                        d_ptr->filesPath.removeAt(j);
                        d_ptr->filesDestination.removeAt(j);
                    }
                    i = d_ptr->filesPath.size();
                    d_ptr->filesPath.clear();
                }
                else if (msgBox.clickedButton()== quit) {
                    d_ptr->filesPath.clear();
                }
            }
        }
    }
}

void FileOperations::process()
{
    int status = 0;
    QString destination;
    quint64 filesSizeCopyStatus = 0;
    quint64 percent = 0;
    quint64 timeRemain = 0;
    QTime wgtWorkTime;
    double copyingSpeed = 0;

    //vars for copied size
    QString signalCopiedSize;
    quint64 sizeCopied = 0;

    wgtWorkTime.start();
    for (int i = 0; i < d_ptr->filesPath.size(); ++i) {
        QFileInfo fileInfo(d_ptr->filesPath.at(i));
        d_ptr->deleteFileList.append(d_ptr->filesPath.at(i));

        if (!fileInfo.isDir()) {
            //input file;
            d_ptr->inputFile.setFileName(d_ptr->filesPath.at(i));
            if (!d_ptr->inputFile.open(QFile::ReadOnly))
            {
                qDebug()<<"Could not open the input file\n"<<d_ptr->inputFile.fileName();
            }
            //initialization path to copy
            destination = d_ptr->filesDestination.at(i); // + fileInfo.fileName();
            emit currentFile("From: " + d_ptr->inputFile.fileName());
            emit currentPath("To: " + destination);

            //div file size to 100;
            int copyPart = d_ptr->inputFile.size() / 100;

            //if file more than 100 megabytes
            if (d_ptr->inputFile.size() > (100 * megaByte))
            {
                d_ptr->outputFile.setFileName(destination);
                if (!d_ptr->outputFile.open(QFile::WriteOnly))
                {
                    qDebug()<<"Could not open the output file\n"<<d_ptr->outputFile.fileName();
                }

                QByteArray dataFromFile;
                quint64 span = 0;

                for (int i = 0; i < 100; ++i) {
                    //break thread
                    if (thread_Break)
                        break;
                    //pause or resume thread
                    d_ptr->sync.lock();
                    if (thread_Pause)
                        d_ptr->pauseCond.wait(&d_ptr->sync); // in this place, your thread will stop to execute until someone calls resume
                    d_ptr->sync.unlock();

                    d_ptr->inputFile.seek(span);
                    span += copyPart;
                    if (i == 100)
                    {
                        dataFromFile = d_ptr->inputFile.readAll();
                        filesSizeCopyStatus += dataFromFile.size();
                    } else {
                        dataFromFile = d_ptr->inputFile.read(copyPart);
                        filesSizeCopyStatus += copyPart;
                    }
                    d_ptr->outputFile.write(dataFromFile);
                    sizeCopied += dataFromFile.size();
                    signalCopiedSize = (QString::number(sizeCopied / megaByte)) + " Mb / " + QString::number(d_ptr->filesSize / megaByte) + " Mb";
                    emit copiedSize(signalCopiedSize);
                    dataFromFile.clear();
                    ++status;

                    percent = (filesSizeCopyStatus * 100) / d_ptr->filesSize;

                    copyingSpeed = (sizeCopied / wgtWorkTime.elapsed());
                    timeRemain = ((d_ptr->filesSize / copyingSpeed) - (wgtWorkTime.elapsed())) / 1000;

                    emit copySpeed(QString("Copying: %1").arg(copyingSpeed) + " KBytes/sec, "
                                   + QString::number(timeRemain) + " sec remain, elapsed: " + QString::number(wgtWorkTime.elapsed() / 1000) + " sec");
                    emit copyFilesStatus(percent);
                    emit copyStatus(status); // !
                }
                status = 0;
                //if file less then 100 megabytes
            } else {
                if (QFile::exists(destination))
                {
                    QFile::remove(destination);
                }
                QFile::copy(d_ptr->filesPath.at(i),destination);
            }
            QString signalCopiedFiles = QString::number(i + 1) + " / " + QString::number(d_ptr->filesPath.size());
            emit copiedFiles(signalCopiedFiles);
            destination.clear();
            d_ptr->outputFile.close();
            d_ptr->inputFile.close();
        }
        if (thread_Break) {
            QFile(d_ptr->filesDestination.at(i)).remove();
        }
    }
    //flag of removing
    if (d_ptr->b_moveFileOperation)
    {
        remDirsFiles(d_ptr->deleteFileList);
    }
    emit formClose(true);
    emit finished();
}

FileOperations::FileOperations(FileOperationsPrivate &dd, QObject *parent)
    :QObject(parent)
    , d_ptr(&dd)
{
    Q_D(FileOperations);
    d->q_ptr = this;
}

void FileOperations::remDirsFiles(const QStringList &sList)
{
    for (int i = 0; i < sList.size(); ++i) {
        if (QFileInfo(sList.at(i)).isFile())
        {
            QFile(sList.at(i)).remove();
        }
        else
        {
            QDir dir(sList.at(i));
            QStringList listOfDirs = dir.entryList(QDir::Dirs |
                                                   QDir::AllDirs |
                                                   QDir::NoDotAndDotDot);
            QStringList listOfFiles = dir.entryList(QDir::Files);
            foreach (QString value, listOfFiles) {
                QString valueAbsolutePath = dir.absolutePath() + "/" + value;
                QFile::setPermissions(valueAbsolutePath, QFile::ReadOwner | QFile::WriteOwner);
                QFile::remove(valueAbsolutePath);
            }
            foreach (QString value, listOfDirs) {
                QStringList valueAbsolutePath;
                valueAbsolutePath.push_back(dir.absolutePath() + "/" + value);
                remDirsFiles(valueAbsolutePath);
            }
            QDir().rmdir(sList.at(i));
        }
        emit formClose(true);
    }
}

FileOperations::~FileOperations()
{

}

//pimpl private sections
FileOperationsPrivate::FileOperationsPrivate()
{
    b_copyFileOperation = false;
    b_moveFileOperation = false;
    b_removeFileOperation = false;
}

FileOperationsPrivate::~FileOperationsPrivate()
{

}

FileOperationWgtPrivate::FileOperationWgtPrivate()
{
    thread = new QThread;
    fileoperations = new FileOperations;
    fileoperations->moveToThread(thread);

    fileCopyStatus = new QProgressBar;
    filesCopyStatus = new QProgressBar;
    fileCopyStatus->setValue(0);
    filesCopyStatus->setValue(0);
    filesCopyStatus->setVisible(false);

    pauseStartBtn = new QPushButton(QObject::tr("Pause"));
    quitBtn = new QPushButton( QObject::tr("Cancel"));
    backgroundBtn = new QPushButton(QObject::tr("Background"));

    copyStatusSpeed = new QLabel(QObject::tr("Copying: "));

    copyFrom = new QLabel(QObject::tr("From: "));
    copyTo = new QLabel(QObject::tr("To: "));

    numberOfFiles = new QLabel(QObject::tr("0/0"));
    numberOfFiles->setVisible(false);

    sizeOfFiles  = new QLabel(QObject::tr("Size:"));
    sizeOfFiles->setVisible(false);

    mainLayout = new QGridLayout();
    mainLayout->addWidget(copyStatusSpeed, 0, 0, 1, 4);
    mainLayout->addWidget(copyFrom, 1, 0, 1, 4);
    mainLayout->addWidget(copyTo, 2, 0, 1, 4);
    mainLayout->addWidget(fileCopyStatus, 3, 0, 1, 4); //first progressBar

    //buttons layout
    hLayout = new QHBoxLayout();
    hLayout->addWidget(pauseStartBtn);
    hLayout->addWidget(quitBtn);
    hLayout->addWidget(backgroundBtn);

    mainLayout->addWidget(filesCopyStatus, 4, 0, 1, 4); // second progressBar
    mainLayout->addWidget(numberOfFiles, 5, 0, 1, 2);
    mainLayout->addWidget(sizeOfFiles, 5, 3, 1, 1);
    mainLayout->addLayout(hLayout, 6, 0, 1, 4); // three buttons
}

FileOperationWgtPrivate::~FileOperationWgtPrivate()
{

}
