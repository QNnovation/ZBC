#include "wgttextview.h"

#include <QPlainTextEdit>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

//constructor
wgtTextView::wgtTextView(QWidget *parent)
    :QMainWindow(parent)
{
    textView = new QPlainTextEdit;

    //color theme
    QPalette p = this->textView->palette();
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Highlight, Qt::green);
    p.setColor(QPalette::HighlightedText, Qt::black);
    this->textView->setPalette(p);

    findReplace = new FindReplaceText();

    createActions();
    createMenu();

    rwMode = true;

    resize(500, 500);
    setCentralWidget(textView);
    setWindowTitle("ZBC viewer/editor");
}

//function for Menu
void wgtTextView::createMenu()
{

    menu = this->menuBar()->addMenu(tr("&File"));
    menu->addAction(saveAct);
    menu->addAction(fileSaveAsAct);
    menu->addSeparator();
    menu->addAction(m_quitAct);

    editMenu = this->menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(findAtTextAct);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
}

//function for Actions
void wgtTextView::createActions()
{
    m_quitAct = new QAction(tr("&Quit"), this);
    m_quitAct->setStatusTip(tr("Close window"));
    m_quitAct->setShortcut(QKeySequence::Close);
    connect(m_quitAct, &QAction::triggered, this, &wgtTextView::close);

    fileSaveAsAct = new QAction(tr("Save as..."), this);
    fileSaveAsAct->setStatusTip(tr("File, save as"));
    fileSaveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(fileSaveAsAct, &QAction::triggered, this, &wgtTextView::saveAs);

    findAtTextAct = new QAction(tr("&Find"), this);
    findAtTextAct->setStatusTip(tr("Find text"));
    findAtTextAct->setShortcut(QKeySequence::Find);
    connect(findAtTextAct, &QAction::triggered, this, &wgtTextView::find);

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setStatusTip(tr("Undo action"));
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, &QAction::triggered, textView, &QPlainTextEdit::undo);

    redoAct = new QAction(tr("Redo"), this);
    redoAct->setStatusTip(tr("Redo action"));
    redoAct->setShortcut(QKeySequence::Redo);
    connect(redoAct, &QAction::triggered, textView, &QPlainTextEdit::redo);

    saveAct = new QAction(tr("Save"), this);
    saveAct->setStatusTip(tr("Save file"));
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &wgtTextView::saveFile);
}

void wgtTextView::replace(QString word, QString newWord, QTextDocument::FindFlags flags)
{
    if (textView->find(word, flags))
        textView->textCursor().insertText(newWord);
}

void wgtTextView::replaceAll(QString word, QString newWord, QTextDocument::FindFlags flags)
{
    while (textView->find(word, flags))
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
    QMessageBox::StandardButton reply = QMessageBox::No;
    if (textView->document()->isModified()) {
        reply = QMessageBox::question(this, "Documet was modified",
                                      "Do you want save?",
                                      QMessageBox::Yes | QMessageBox::No);
    }
    if (reply == QMessageBox::Yes)
    {
        this->saveFile();
        findReplace->close();
    }
    else {
        findReplace->close();
    }
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

//find slot
void wgtTextView::find()
{
    findReplace->isReplace(!rwMode);
    findReplace->show();
    connect(findReplace, &FindReplaceText::findSignal, this, &wgtTextView::findSlot);
    if (!rwMode) {
        connect(findReplace, &FindReplaceText::replaceSignal, this, &wgtTextView::replaceSlot);
        connect(findReplace, &FindReplaceText::replaceAllSignal, this, &wgtTextView::replaceAllSlot);
    }
}

void wgtTextView::findSlot(QString word, QTextDocument::FindFlags flags)
{
    textView->find(word, flags);
}

void wgtTextView::replaceSlot(QString word, QTextDocument::FindFlags flags, QString newWord)
{
    replace(word, newWord, flags);
}

void wgtTextView::replaceAllSlot(QString word, QTextDocument::FindFlags flags, QString newWord)
{
    replaceAll(word, newWord, flags);
}

wgtTextView::~wgtTextView()
{

}

