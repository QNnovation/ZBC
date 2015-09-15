#include "wgttextview.h"

#include <QPlainTextEdit>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

//constructor
wgtTextView::wgtTextView(QWidget *parent)
    :QMainWindow(parent)
{
    createActions();
    createMenu();

    textView = new QPlainTextEdit;
    rwMode = true;

    resize(500, 500);
    setCentralWidget(textView);
    setWindowTitle("ZBC viewer/editor");
}

//function for Menu
void wgtTextView::createMenu()
{

    menu = this->menuBar()->addMenu(tr("&File"));
    menu->addAction(fileSaveAsAct);
    menu->addSeparator();
    menu->addAction(quitAct);

    editMenu = this->menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(findAtTextAct);
}

//function for Actions
void wgtTextView::createActions()
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setStatusTip(tr("Close window"));
    quitAct->setShortcut(QKeySequence::Close);
    connect(quitAct, &QAction::triggered, this, &wgtTextView::close);

    fileSaveAsAct = new QAction(tr("Save as..."), this);
    fileSaveAsAct->setStatusTip(tr("File, save as"));
    fileSaveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(fileSaveAsAct, &QAction::triggered, this, &wgtTextView::saveAs);

    findAtTextAct = new QAction(tr("&Find"), this);
    findAtTextAct->setStatusTip(tr("Find text"));
    findAtTextAct->setShortcut(QKeySequence::Find);
    connect(findAtTextAct, &QAction::triggered, this, &wgtTextView::find);
}

void wgtTextView::replaceText(QString word, QString newWord, QTextDocument::FindFlags flags)
{
    if (textView->find(word, flags))
    textView->textCursor().insertText(newWord);
}

//function loadFile
bool wgtTextView::loadFile(const QString &filePath, char mode)
{
    pathToFile = filePath;
    if (mode == 'w')
        rwMode = false;

    QByteArray dataFromFile;
    QFile inputFile(pathToFile);
    if (!inputFile.open(QFile::ReadOnly))
    {
        qDebug()<<"Could not open the input file\n"<<pathToFile;
        return false;
    } else {
        dataFromFile = inputFile.readAll();
        textView->setReadOnly(rwMode);
        textView->setPlainText(dataFromFile);
        return true;
    }
}

void wgtTextView::viewFile(QString &filePath)
{
    loadFile(filePath);
}

void wgtTextView::editFile(QString &filePath)
{
    loadFile(filePath, 'w');
}

void wgtTextView::closeEvent(QCloseEvent *)
{
    findReplace->close();
}

//slot saveAs
bool wgtTextView::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QObject::tr("Save File"),
                                                    pathToFile,
                                                    QObject::tr("All files (*.*)"));
    pathToFile = fileName;
    return saveFile();
}

//slot saveFile
bool wgtTextView::saveFile()
{
    QFile inputFile(pathToFile);
    if (!inputFile.open(QFile::WriteOnly)) {
        qDebug() << "Could not open the output file\n"<<pathToFile;
        return false;
    }
    else {
        QString buffer = textView->toPlainText();
        QTextStream stream(&inputFile);
        stream << buffer;
        buffer.clear();
        inputFile.close();
        return true;
    }
}

void wgtTextView::find()
{
    if (rwMode)
        findReplace = new FindReplaceText();
    else
        findReplace = new FindReplaceText(true);
    findReplace->show();
    connect(findReplace, &FindReplaceText::findTextOptionsSig, this, &wgtTextView::getFindReplace);
}

void wgtTextView::getFindReplace(QString word, QTextDocument::FindFlags flags, QString newWord)
{
    if (rwMode) {
    textView->find(word, flags);
    }
    else {
    //реализовать функцию заменить один раз
    replaceText(word, newWord, flags);
    //реализовать функцию заменить всё
    }
}

wgtTextView::~wgtTextView()
{

}

