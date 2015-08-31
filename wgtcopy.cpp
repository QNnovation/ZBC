/*
 * Created by Lysytskyi Oleksii 2015
*/

#include "wgtcopy.h"
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

FileOperationWgt::FileOperationWgt(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    setWindowTitle("ZBC Copying");
    setFixedSize(450, 128); //125 175

    thread = new QThread;
    fileoperations = new FileOperations;
    fileoperations->moveToThread(thread);

    fileCopyStatus = new QProgressBar;
    filesCopyStatus = new QProgressBar;
    fileCopyStatus->setValue(0);
    filesCopyStatus->setValue(0);
    filesCopyStatus->setVisible(false);

    pauseStartBtn = new QPushButton(tr("Pause"));
    quitBtn = new QPushButton(tr("Cancel"));
    backgroundBtn = new QPushButton(tr("Background"));

    copyStatusSpeed = new QLabel(tr("Copying: "));

    copyFrom = new QLabel(tr("From: "));
    copyTo = new QLabel(tr("To: "));

    numberOfFiles = new QLabel(tr("0/0"));
    numberOfFiles->setVisible(false);

    sizeOfFiles  = new QLabel(tr("Size:"));
    sizeOfFiles->setVisible(false);

    mainLayout = new QGridLayout(this);
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
    setLayout(mainLayout);

    connect(quitBtn, SIGNAL(clicked()), this, SLOT(closeWgt()));
    connect(pauseStartBtn, SIGNAL(clicked()), this, SLOT(threadPauseResume()));
    connect(backgroundBtn, SIGNAL(clicked()), this, SLOT(toBackground()));
    connect(fileoperations, SIGNAL(copyStatus(int)), fileCopyStatus, SLOT(setValue(int)));
    connect(fileoperations, SIGNAL(copyFilesStatus(int)), filesCopyStatus, SLOT(setValue(int)));
    connect(fileoperations, SIGNAL(copySpeed(QString)), copyStatusSpeed, SLOT(setText(QString)));
    connect(fileoperations, SIGNAL(currentFile(QString)), copyFrom, SLOT(setText(QString)));
    connect(fileoperations, SIGNAL(currentPath(QString)), copyTo, SLOT(setText(QString)));
    connect(fileoperations, SIGNAL(copiedFiles(QString)), numberOfFiles, SLOT(setText(QString)));
    connect(fileoperations, SIGNAL(copiedSize(QString)), sizeOfFiles, SLOT(setText(QString)));

    //connect signals from copy to gui
    connect(fileoperations, SIGNAL(finished()), this, SIGNAL(finishOperation()));

    //mtoxa
    connect(fileoperations, SIGNAL(currentPath(QString)), this, SLOT(currentFileName(QString)));

    //connect thread start signal
    connect(thread, SIGNAL(started()), fileoperations, SLOT(process()));
    //Right thread exit
    connect(fileoperations, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    //close widget window
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(fileoperations, SIGNAL(formClose(bool)), this, SLOT(closeWgt()));
}

//copy files
void FileOperationWgt::copyFileOperation(const QStringList& files, const QString& path)
{
    //qDebug() << "Copy operation detected!";
    if (confirmOperation())
    {
        fileoperations->loadFiles(files, path, 'c');
        if (fileoperations->numFiles() >  1)
        {
            setFixedSize(450, 175);
            filesCopyStatus->setVisible(true);
            sizeOfFiles->setVisible(true);
            numberOfFiles->setVisible(true);
        }
        //start thread
        thread->start();
    }
}

//move files
void FileOperationWgt::moveFileOperation(const QStringList& files, const QString& path)
{
    if (confirmOperation())
    {
        fileoperations->loadFiles(files, path, 'm');
        if (fileoperations->numFiles() >  1)
        {
            setFixedSize(450, 175);
            filesCopyStatus->setVisible(true);
            sizeOfFiles->setVisible(true);
            numberOfFiles->setVisible(true);
        }
        //start thread
        thread->start();
    }
}

//remove files
void FileOperationWgt::removeFileOperation(const QStringList& files)
{
    if (confirmOperation())
    {
        copyStatusSpeed->setText(tr("Deleting"));
        copyTo->setVisible(false);
        numberOfFiles->setVisible(false);
        sizeOfFiles->setVisible(false);
        filesCopyStatus->setVisible(false);
        fileoperations->remDirsFiles(files);
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
}

void FileOperationWgt::threadPauseResume()
{
    if (!fileoperations->thread_Pause)
        fileoperations->Pause();
    else
        fileoperations->Resume();
}

void FileOperationWgt::closeWgt()
{
    fileoperations->Stop();
    this->close();
}

void FileOperationWgt::currentFileName(QString filePath)
{
    emit curFile(filePath.remove(0, 4));
}

void FileOperationWgt::closeEvent(QCloseEvent *event)
{
    fileoperations->Stop();
}

void FileOperationWgt::toBackground()
{
    showMinimized();
}

FileOperationWgt::~FileOperationWgt()
{

}

//FileOperations class implementation***/

FileOperations::FileOperations() : QObject()
{
    b_copyFileOperation = false;
    b_moveFileOperation = false;
    b_removeFileOperation = false;
    thread_Pause = false;
    thread_Break = false;
}

bool FileOperations::Resume()
{
    sync.lock();
    thread_Pause = false;
    sync.unlock();
    pauseCond.wakeAll();
    return thread_Pause;
}

bool FileOperations::Pause()
{
    sync.lock();
    thread_Pause = true;
    sync.unlock();
    return thread_Pause;
}

bool FileOperations::Stop()
{
    thread_Break = true;
    return thread_Break;
}

//checking for free space
bool FileOperations::spaceCheck(QString path, quint64 size)
{
    ULARGE_INTEGER  freeSpace, totalSpace, p3;
    LPCTSTR source = (LPCWSTR)path.utf16(); // L"P:\\";
    if (GetDiskFreeSpaceEx(source, &freeSpace, &totalSpace, &p3))
    {
        if (size > freeSpace.QuadPart)
            return false;
        else
            return true;
    }
}

void FileOperations::copyDirs(const QDir &dir)
{
    // !Folders creation;
    //create root directory;
    if (!QDir().exists(fileDestination + dir.dirName()))
        QDir().mkdir(fileDestination + dir.dirName());

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
    for (int i = 0; i < fullPath.size(); ++i)
    {
        if (QFileInfo(fullPath.at(i)).isDir())
        {
            fullPath[i].remove(0, QString(dir.path()).size() - QString(dir.dirName()).size());
            dirCreate.push_back(fileDestination + fullPath.at(i));
        }
    }
    //Create folders;
    for (int i = 0; i < dirCreate.size(); ++i)
    {
        if (!QDir().exists(dirCreate.at(i)))
            QDir().mkdir(dirCreate.at(i));
    }
    // !Add files;
    for (int i = 0; i < fullPath.size(); ++i)
    {
        if (QFileInfo(fullPath.at(i)).isFile())
        {
            filesPath.append(fullPath.at(i));
            filesDestination.append(fileDestination + fullPath[i].remove(0, QString(dir.path()).size() - QString(dir.dirName()).size()));
        }
    }
}

void FileOperations::loadFiles(QStringList files_Path, const QString files_Destination, char act)
{
    //qDebug() << "Load files operation detected!";
    //checking copy file to self
    QString checkPath;
    for (int i = 0; i < files_Path.size(); ++i)
    {
        checkPath = files_Path.at(i);
        checkPath = checkPath.left(checkPath.lastIndexOf('/')) + "/";
        //qDebug() << "Copy to itself check Destination:" << files_Destination << "Checkpath: " << checkPath;
        if (checkPath == files_Destination)
        {
            QMessageBox::warning(0, "Abort", "You can not copy a file to itself!");
            files_Path.clear();
        }
    }
    //check flag
    switch(act)
    {
    case 'c':
        b_copyFileOperation = true;
        break;
    case 'm':
        b_moveFileOperation = true;
        break;
    case 'r':
        b_removeFileOperation = true;
        break;
    }

    //check input data, then create path for files
    if (files_Path.isEmpty() && files_Destination.isEmpty())
        return;

    fileDestination = files_Destination;

    for (int i = 0; i < files_Path.size(); ++i)
    {
        //qDebug() << "Input files:"<<files_Path.at(i);
        if (QFileInfo(files_Path.at(i)).isFile())
        {
            filesPath.push_back(files_Path.at(i));
            filesDestination.push_back(files_Destination + QFileInfo(files_Path.at(i)).fileName());
        }
        else
        {
            filesPath.push_back(files_Path.at(i));
            filesDestination.push_back(files_Destination);
            deleteDirList.append(files_Path.at(i));
        }
    }

    //if dir, add files to array
    for (int i = 0; i < filesPath.size(); ++i)
    {
        if (QFileInfo(filesPath.at(i)).isDir())
        {
            copyDirs(QDir(filesPath.at(i)));
        }
    }
    //get the total size of files
    filesSize = 0;
    for (int j = 0; j < filesPath.size(); ++j)
    {
        filesSize += QFileInfo(filesPath.at(j)).size();
    }

    //check for free space
    QString path = fileDestination;
    path = fileDestination.left(2);

    if (!spaceCheck(path, filesSize))
    {
        QMessageBox::warning(0, "Abort", "No more space");
        filesPath.clear();
    }
    //overwrite check
    for(int i = 0; i < filesPath.size(); ++i)
    {
        QFileInfo info(filesPath.at(i));
        QString checkFile = filesDestination.at(i);
        //qDebug() << "File overwrite:" << checkFile << filesPath.at(i);
        if (info.isFile())
        {
            if (QFile::exists(checkFile))
            {
                QMessageBox msgBox;
                msgBox.setText(filesPath.at(i));
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
                if(msgBox.clickedButton()== change)
                {

                }
                else if(msgBox.clickedButton()== changeAll)
                {
                    i = filesPath.size();
                }

                else if(msgBox.clickedButton()== skip)
                {
                    filesPath.removeAt(i);
                    filesDestination.removeAt(i);
                    i = 0;
                }
                else if(msgBox.clickedButton()== skipAll)
                {
                    for (int j = i; j < filesPath.size(); ++j)
                    {
                        filesPath.removeAt(j);
                        filesDestination.removeAt(j);
                    }
                    i = filesPath.size();
                    filesPath.clear();
                }
                else if(msgBox.clickedButton()== quit)
                {
                    filesPath.clear();
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
    for(int i = 0; i < filesPath.size(); ++i)
    {
        QFileInfo fileInfo(filesPath.at(i));
        deleteFileList.append(filesPath.at(i));

        if (!fileInfo.isDir())
        {
            //input file;
            inputFile.setFileName(filesPath.at(i));
            if (!inputFile.open(QFile::ReadOnly))
            {
                qDebug()<<"Could not open the input file\n"<<inputFile.fileName();
            }
            //initialization path to copy
            destination = filesDestination.at(i); // + fileInfo.fileName();
            emit currentFile("From: " + inputFile.fileName());
            emit currentPath("To: " + destination);

            //div file size to 100;
            int copyPart = inputFile.size() / 100;

            //if file more than 100 megabytes
            if (inputFile.size() > (100 * megaByte))
            {
                //qDebug()<<i<<"Filepatch:"<<filesPath.at(i)<<"FilesDestination:"<<filesDestination.at(i)<<"Filesize: "<<fileInfo.size();
                outputFile.setFileName(destination);
                if (!outputFile.open(QFile::WriteOnly))
                {
                    qDebug()<<"Could not open the output file\n"<<outputFile.fileName();
                }

                QByteArray dataFromFile;
                quint64 span = 0;

                for (int i = 0; i < 100; ++i)
                {
                    //break thread
                    if (thread_Break)
                        break;
                    //pause or resume thread
                    sync.lock();
                    if(thread_Pause)
                        pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
                    sync.unlock();

                    inputFile.seek(span);
                    span += copyPart;
                    if (i == 100)
                    {
                        dataFromFile = inputFile.readAll();
                        filesSizeCopyStatus += dataFromFile.size();
                    }
                    else
                    {
                        dataFromFile = inputFile.read(copyPart);
                        filesSizeCopyStatus += copyPart;
                    }
                    outputFile.write(dataFromFile);
                    sizeCopied += dataFromFile.size();
                    signalCopiedSize = (QString::number(sizeCopied / megaByte)) + " Mb / " + QString::number(filesSize / megaByte) + " Mb";
                    emit copiedSize(signalCopiedSize);
                    dataFromFile.clear();
                    ++status;

                    percent  = (filesSizeCopyStatus * 100) / filesSize;

                    copyingSpeed = (sizeCopied / wgtWorkTime.elapsed());
                    timeRemain = ((filesSize / copyingSpeed) - (wgtWorkTime.elapsed())) / 1000;

                    emit copySpeed(QString("Copying: %1").arg(copyingSpeed) + " KBytes/sec, "
                                   + QString::number(timeRemain) + " sec remain, elapsed: " + QString::number(wgtWorkTime.elapsed() / 1000) + " sec");
                    emit copyFilesStatus(percent);
                    emit copyStatus(status); // !
                }
                status = 0;
            }

            //if file less then 100 megabytes
            else
            {
                qDebug()<<i<<"Filepatch:"<<filesPath.at(i)<<"FilesDestination:"<<filesDestination.at(i)<<"Filesize: "<<fileInfo.size();
                if (QFile::exists(destination))
                {
                    QFile::remove(destination);
                }
                QFile::copy(filesPath.at(i),destination);
            }

            QString signalCopiedFiles = QString::number(i + 1) + " / " + QString::number(filesPath.size());
            emit copiedFiles(signalCopiedFiles);
            destination.clear();
            outputFile.close();
            inputFile.close();
        }
        if (thread_Break)
            QFile(filesDestination.at(i)).remove();
    }
    //flag of removing
    if (b_moveFileOperation)
    {
        remDirsFiles(deleteFileList);
    }
    emit formClose(true);
    emit finished();
}

void FileOperations::remDirsFiles(const QStringList &sList)
{
    for (int i = 0; i < sList.size(); ++i)
    {
        if (QFileInfo(sList.at(i)).isFile())
        {
            //qDebug() << i << "Files deleted:" <<sList.at(i);
            QFile(sList.at(i)).remove();
        }
        else
        {
            //qDebug() << i << "Directories deleted:" <<sList.at(i);
            QDir(sList.at(i)).removeRecursively();
        }
    }
    emit formClose(true);
}

FileOperations::~FileOperations()
{

}
