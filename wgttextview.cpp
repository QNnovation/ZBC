#include "wgttextview.h"

#include <QPlainTextEdit>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QTextBlock>
#include <QStatusBar>
#include <QLabel>
#include <QFileInfo>

//constructor
wgtTextView::wgtTextView(QWidget *parent)
    :QMainWindow(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    m_textView = new editor;

    //color theme
    QPalette p = this->m_textView->palette();
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Highlight, Qt::green);
    p.setColor(QPalette::HighlightedText, Qt::black);
    this->m_textView->setPalette(p);

    m_findReplace = new FindReplaceText();

    m_statusLbl = new QLabel(tr("status"));
    m_statusLbl->setText(m_textView->getInfo());
    statusBar()->addWidget(m_statusLbl);

    createActions();
    createMenu();

    m_rwMode = true;

    resize(500, 500);
    setCentralWidget(m_textView);
    setWindowTitle("ZBC viewer/editor");
}

//function for Menu
void wgtTextView::createMenu()
{

    m_menu = this->menuBar()->addMenu(tr("&File"));
    m_menu->addAction(m_saveAct);
    m_menu->addAction(m_fileSaveAsAct);
    m_menu->addSeparator();
    m_menu->addAction(m_quitAct);

    m_editMenu = this->menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_findAtTextAct);
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);
}

//function for Actions
void wgtTextView::createActions()
{
    m_quitAct = new QAction(tr("&Quit"), this);
    m_quitAct->setStatusTip(tr("Close window"));
    m_quitAct->setShortcut(QKeySequence::Close);
    m_quitAct->setShortcut(Qt::Key_Escape);
    connect(m_quitAct, &QAction::triggered, this, &wgtTextView::close);

    m_fileSaveAsAct = new QAction(tr("Save as..."), this);
    m_fileSaveAsAct->setStatusTip(tr("File, save as"));
    m_fileSaveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(m_fileSaveAsAct, &QAction::triggered, this, &wgtTextView::saveAs);

    m_findAtTextAct = new QAction(tr("&Find"), this);
    m_findAtTextAct->setStatusTip(tr("Find text"));
    m_findAtTextAct->setShortcut(QKeySequence::Find);
    connect(m_findAtTextAct, &QAction::triggered, this, &wgtTextView::find);

    m_undoAct = new QAction(tr("Undo"), this);
    m_undoAct->setStatusTip(tr("Undo action"));
    m_undoAct->setShortcut(QKeySequence::Undo);
    connect(m_undoAct, &QAction::triggered, m_textView, &QPlainTextEdit::undo);

    m_redoAct = new QAction(tr("Redo"), this);
    m_redoAct->setStatusTip(tr("Redo action"));
    m_redoAct->setShortcut(QKeySequence::Redo);
    connect(m_redoAct, &QAction::triggered, m_textView, &QPlainTextEdit::redo);

    m_saveAct = new QAction(tr("Save"), this);
    m_saveAct->setStatusTip(tr("Save file"));
    m_saveAct->setShortcut(QKeySequence::Save);
    connect(m_saveAct, &QAction::triggered, this, &wgtTextView::saveFile);
}

void wgtTextView::replace(const QString &word, const QString &newWord, QTextDocument::FindFlags flags)
{
    if (m_textView->find(word, flags))
        m_textView->textCursor().insertText(newWord);
}

void wgtTextView::replaceAll(const QString &word, const QString &newWord, QTextDocument::FindFlags flags)
{
    while (m_textView->find(word, flags))
        m_textView->textCursor().insertText(newWord);

}

//function loadFile
bool wgtTextView::loadFile(const QString &filePath, char mode)
{
    pathToFile = filePath;
    if (mode == 'w')
        m_rwMode = false;

    QByteArray dataFromFile;
    QFile inputFile(pathToFile);
    if (!inputFile.open(QFile::ReadOnly))
    {
        qDebug()<<"Could not open the input file\n"<<pathToFile;
        return false;
    } else {
        dataFromFile = inputFile.readAll();
        m_textView->setReadOnly(m_rwMode);
        m_textView->setPlainText(dataFromFile);
        return true;
    }
}

void wgtTextView::viewFile(const QString &filePath)
{
    loadFile(filePath);
}

void wgtTextView::editFile(const QString &filePath)
{
    loadFile(filePath, 'w');
}

void wgtTextView::closeEvent(QCloseEvent *)
{
    QMessageBox::StandardButton reply = QMessageBox::No;
    if (m_textView->document()->isModified()) {
        reply = QMessageBox::question(this, tr("Documet was modified"),
                                      tr("Do you want save?"),
                                      QMessageBox::Yes | QMessageBox::No);
    }
    if (reply == QMessageBox::Yes)
    {
        this->saveFile();
    }
    else {
        m_findReplace->close();
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
        QString buffer = m_textView->toPlainText();
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
    m_findReplace->isReplace(!m_rwMode);
    m_findReplace->show();
    connect(m_findReplace, &FindReplaceText::findSignal, this, &wgtTextView::findSlot);
    if (!m_rwMode) {
        connect(m_findReplace, &FindReplaceText::replaceSignal, this, &wgtTextView::replaceSlot);
        connect(m_findReplace, &FindReplaceText::replaceAllSignal, this, &wgtTextView::replaceAllSlot);
    }
}

void wgtTextView::findSlot(const QString &word, QTextDocument::FindFlags flags)
{
    m_textView->find(word, flags);
}

void wgtTextView::replaceSlot(const QString &word, QTextDocument::FindFlags flags, const QString &newWord)
{
    replace(word, newWord, flags);
}

void wgtTextView::replaceAllSlot(const QString &word, QTextDocument::FindFlags flags, const QString &newWord)
{
    replaceAll(word, newWord, flags);
}

wgtTextView::~wgtTextView()
{
    delete m_findReplace;
}


//editor class implementation
editor::editor(QWidget *parent) : QPlainTextEdit(parent)
{
    m_editorPaintArea = new editorPaintArea(this);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &editor::updateLinesWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &editor::updateLineNumberArea);
    //connect(m_textFindEdit, &QLineEdit::textChanged, this, &FindReplaceText::isEmptyText);
    updateLinesWidth(0);
}

void editor::lineNumberPaint(QPaintEvent *event)
{
    QPainter painter(m_editorPaintArea);

    painter.fillRect(event->rect(), Qt::gray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            QFont fnt("Arial", 8, QFont::Bold);
            painter.setPen(Qt::darkBlue);
            painter.setFont(fnt);
            painter.drawText(0, top, m_editorPaintArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int editor::linesNumberWidth()
{
    int areaWidth = blockCount();
    int cnt = 1;
    while (areaWidth >= 10) {
        areaWidth /= 10;
        ++cnt;
    }
    int size = (fontMetrics().width(QLatin1Char('9')) * cnt) + 3;
    return size;
}

QString editor::getInfo()
{
    //int number1 = 0; //blockCount();
    QString info;

    return info;
}

void editor::updateLinesWidth(int)
{
    setViewportMargins(linesNumberWidth(), 0, 0, 0);
}

void editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_editorPaintArea->scroll(0, dy);
    else
        m_editorPaintArea->update(0, rect.y(), m_editorPaintArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLinesWidth(0);
}

void editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect content = contentsRect();
    m_editorPaintArea->setGeometry(QRect(content.left(), content.top(),
                                         linesNumberWidth(), content.height()));
}
