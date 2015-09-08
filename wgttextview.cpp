#include "wgttextview.h"
#include "wgttextview_p.h"

#include <QPlainTextEdit>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

wgtTextView::wgtTextView(QWidget *parent)
    :QMainWindow(parent)
    , d_ptr(new wgtTextViewPrivate())
{
    Q_D(wgtTextView);
    d->q_ptr = this;
    resize(500, 500);
    setCentralWidget(d->textView);
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
