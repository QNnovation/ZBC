#include <QtWidgets>

#include "wgttextview.h"

wgtTextView::wgtTextView()
{
    textView = new QPlainTextEdit;
    setCentralWidget(textView);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    connect(textView->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    setCurrentFile("");
}

void wgtTextView::viewFile(const QString &fileName)
{
    loadFile(fileName);
    textView->setReadOnly(true);
}

void wgtTextView::editFile(const QString &fileName)
{
    loadFile(fileName);
    textView->setReadOnly(false);
}


void wgtTextView::open()
{
    currentFile = QFileDialog::getOpenFileName(this);
    if (!currentFile.isEmpty())
        loadFile(currentFile);
}

void wgtTextView::newFile()
{
    if(maybeSave())
    {
        textView->clear();
    }
}

bool wgtTextView::save()
{
    if(currentFile.isEmpty())
    {
        return saveAs();
    }
    else
    {
        return saveFile(currentFile);
    }
}

bool wgtTextView::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;
    return saveFile(files.at(0));
}

void wgtTextView::documentWasModified()
{
    setWindowModified(textView->document()->isModified());
}

void wgtTextView::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a existing file"));
    connect(openAct, SIGNAL(triggered(bool)), this, SLOT(open()));

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered(bool)), SLOT(newFile()));

    exitAct = new QAction(tr("&Quit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit program"));
    connect(exitAct, SIGNAL(triggered(bool)), this, SLOT(close()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save file"));
    connect(saveAct, SIGNAL(triggered(bool)), SLOT(save()));

    saveAsAct = new QAction(tr("&Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save file as..."));
    connect(saveAsAct, SIGNAL(triggered(bool)), SLOT(saveAs()));

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy text"));
    connect(copyAct, SIGNAL(triggered(bool)), textView, SLOT(copy()));

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste text"));
    connect(pasteAct, SIGNAL(triggered(bool)), textView, SLOT(paste()));

    cutAct = new QAction(tr("Cut"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut text"));
    connect(cutAct, SIGNAL(triggered(bool)), textView, SLOT(cut()));
}

void wgtTextView::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(cutAct);
}

void wgtTextView::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void wgtTextView::createStatusBar()
{
    statusBar()->showMessage(currentFile);
}

bool wgtTextView::maybeSave()
{
    if (textView->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document was modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}


void wgtTextView::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textView->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    statusBar()->showMessage(tr("File %1 loaded").arg(file.fileName()));
}

bool wgtTextView::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out<<textView->toPlainText();
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void wgtTextView::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    textView->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFile;
    if (shownName.isEmpty())
        shownName = "unnamed.txt";
    setWindowFilePath(shownName);
}
