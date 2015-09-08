#include "wgttextview.h"
#include "wgttextview_p.h"

#include <QPlainTextEdit>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>

wgtTextView::wgtTextView(QWidget *parent)
    :QMainWindow(parent)
    , d_ptr(new wgtTextViewPrivate())
{
    Q_D(wgtTextView);
    d->q_ptr = this;
    d->createMenu();
    resize(500, 500);
    setCentralWidget(d->textView);
    setWindowTitle("ZBC viewer/editor");
}

wgtTextView::~wgtTextView()
{

}

wgtTextView::loadFile(const QString &filePath, char mode)
{
    Q_D(wgtTextView);
    d->pathToFile = filePath;
    if (mode == 'w')
        d->rwMode = false;
    d->readFile();
}

wgtTextView::wgtTextView(wgtTextViewPrivate &dd, QWidget *parent)
    : QMainWindow(parent)
    , d_ptr(&dd)
{
    Q_D(wgtTextView);
    d->q_ptr = this;
}

//wgtTextViewPrivate class
wgtTextViewPrivate::wgtTextViewPrivate()
{
    textView = new QPlainTextEdit;
    rwMode = true;
}

void wgtTextViewPrivate::readFile()
{
    if (pathToFile.isEmpty())
        return;
    QByteArray dataFromFile;
    QFile inputFile(pathToFile);
    if (!inputFile.open(QFile::ReadOnly))
    {
        qDebug()<<"Could not open the input file\n"<<pathToFile;
    }
    dataFromFile = inputFile.readAll();
    textView->setReadOnly(rwMode);
    textView->setPlainText(dataFromFile);
}

wgtTextViewPrivate::~wgtTextViewPrivate()
{

}

void wgtTextViewPrivate::createActions()
{
    Q_Q(wgtTextView);
    quitAct = new QAction(QObject::tr("&Quit"), q_ptr);
    quitAct->setStatusTip(QObject::tr("Close window"));
    quitAct->setShortcut(QKeySequence::Close);
    QObject::connect(quitAct, SIGNAL(triggered(bool)), q_ptr, SLOT(close()));
}

void wgtTextViewPrivate::createMenu()
{
    Q_Q(wgtTextView);
    menu = q_ptr->menuBar()->addMenu(QObject::tr("&File"));
    menu->addAction(quitAct);


}
