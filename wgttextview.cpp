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
#include <QSettings>

#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewWidget>
#include <QPainter>
#include <QPaintDevice>
#include <QFontDialog>

qint32 const kiloByte = 1024;

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
    m_docInfoLbl = new QLabel(tr("empty"));
    m_fileInfoLbl = new QLabel(tr("empty"));

    connect(m_textView, &editor::info, m_docInfoLbl, &QLabel::setText);

    statusBar()->addWidget(m_docInfoLbl);
    statusBar()->addWidget(m_fileInfoLbl);

    createActions();
    createMenu();

    m_rwMode = true;
    readSettings();
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
    m_menu->addAction(m_previewAct);
    m_menu->addAction(m_printAct);
    m_menu->addSeparator();
    m_menu->addAction(m_quitAct);

    m_editMenu = this->menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_findAtTextAct);
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_fontAct);
}

//function for Actions
void wgtTextView::createActions()
{
    m_fontAct = new QAction(tr("Font"), this);
    m_fontAct->setStatusTip(tr("Change font of document"));
    connect(m_fontAct, &QAction::triggered, this, &wgtTextView::changeFont);

    m_previewAct = new QAction(tr("Preview document"), this);
    m_previewAct->setStatusTip(tr("Watch the document before print"));
    connect(m_previewAct, &QAction::triggered, this, &wgtTextView::previewDialog);

    m_printAct = new QAction(tr("&Print"), this);
    m_printAct->setStatusTip(tr("Print document"));
    m_printAct->setShortcut(QKeySequence::Print);
    connect(m_printAct, &QAction::triggered, this, &wgtTextView::printDoc);

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
    m_fileInfoLbl->setText("File: " + QFileInfo(pathToFile).fileName()
                           + " Size: " + QString::number(QFileInfo(pathToFile).size() / kiloByte)
                           + " Kb");
    if (mode == 'w')
        m_rwMode = false;

    QStringList encodeS = { "Big5", "Big5-HKSCS", "CP949", "EUC-JP" "EUC-KR", "GB18030", "HP-ROMAN8",
                            "IBM 850", "IBM 866", "IBM 874", "ISO 2022-JP", "ISO 8859-1", "8859-2", "8859-3",
                            "8859-4", "8859-5", "8859-6", "8859-7", "8859-8", "8859-9", "8859-10", "ISO 8859-13",
                            "ISO 8859-14", "ISO 8859-15", "ISO 8859-16", "KOI8-R", "KOI8-U", "Macintosh",
                            "Shift-JIS", "TIS-620", "TSCII", "UTF-8", "UTF-16", "UTF-16BE", "UTF-16LE", "UTF-32",
                            "UTF-32BE", "UTF-32LE", "Windows-1250", "Windows-1251", "Windows-1252", "Windows-1253",
                            "Windows-1254", "Windows-1255", "Windows-1256", "Windows-1257", "Windows-1258" };
    QFile inputFile(pathToFile);

#ifndef Q_WS_WIN
QString encode("Windows-1251");
#endif
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Could not open the input file\n"<<pathToFile;
        return false;
    } else {
        m_lastOpenFiles.push_back(filePath);
        while(!inputFile.atEnd())
        {
            QString buf = inputFile.readLine();
            for (int i = 0; i < encodeS.size(); ++i) {
                if (buf.contains(encodeS.at(i))) {
                    encode.clear();
                    encode = encodeS.at(i);
                    break;
                }
            }
        }
    }
    inputFile.seek(0);
    QTextStream dataFromFile(&inputFile);
    dataFromFile.setCodec(encode.toLatin1());
    m_textView->setReadOnly(m_rwMode);
    m_textView->setPlainText(dataFromFile.readAll());
    return true;
}

void wgtTextView::viewFile(const QString &filePath)
{
    loadFile(filePath);
}

void wgtTextView::editFile(const QString &filePath)
{
    loadFile(filePath, 'w');
}

void wgtTextView::writeSettings()
{
    QFont editorFont = m_textView->font();
    m_settings.beginGroup("/Settings/TextViewEdit");
    m_settings.setValue("editor.Width", width());
    m_settings.setValue("editor.Height", height());
    m_settings.setValue("editor.Font", editorFont.toString());
    m_settings.setValue("editor.FontSize", editorFont.pointSize());
    m_settings.setValue("editor.FontBold", editorFont.bold());
    m_settings.setValue("editor.Italic", editorFont.italic());
    m_settings.setValue("editor.lastOpenFiles", m_lastOpenFiles);
    m_settings.endGroup();
}

void wgtTextView::readSettings()
{
    m_settings.beginGroup("/Settings/TextViewEdit");
    //read properties
    int nWidth = m_settings.value("editor.Width", width()).toInt();
    int nHeight = m_settings.value("editor.Height", height()).toInt();
    //m_lastOpenFiles = m_settings.value("editor.lastOpenFiles");
    QString fontFamily = m_settings.value("editor.Font", m_textView->font()).toString();
    int fontSize = m_settings.value("editor.FontSize", 12).toInt();
    bool fontIsBold = m_settings.value("editor.FontBold", false).toBool();
    bool fontIsItalic = m_settings.value("editor.Italic", false).toBool();
    //set properties
    QFont editorFont(fontFamily, fontSize, QFont::Bold, fontIsItalic);
    editorFont.setBold(fontIsBold);
    resize(nWidth, nHeight);
    m_textView->setFont(editorFont);
    m_settings.endGroup();
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

bool wgtTextView::printDoc()
{
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        m_textView->print(&printer);
        return true;
    }
    return false;
}

void wgtTextView::previewDialog()
{
    QPrintPreviewDialog preview(&printer, this, Qt::WindowFullscreenButtonHint
                                | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint
                                | Qt::WindowCloseButtonHint);
    connect(&preview, QPrintPreviewDialog::paintRequested, this, &wgtTextView::paintPreview);
    preview.exec();
}

void wgtTextView::paintPreview(QPrinter *printer)
{
    m_textView->print(printer);
}

void wgtTextView::changeFont()
{
    bool ok;
    QFont fnt = QFontDialog::getFont(&ok);
    m_textView->setFont(fnt);
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
    writeSettings();
}


//editor class implementation
editor::editor(QWidget *parent) : QPlainTextEdit(parent)
{
    m_editorPaintArea = new editorPaintArea(this);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &editor::updateLinesWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &editor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &editor::docInfo);

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

void editor::docInfo()
{
    QString data;
    //number of lines
    int linesArea = blockCount();
    data += "Lines: " + (QString::number(linesArea));
    //current line and column
    QTextCursor cursor = this->textCursor();
    int y = cursor.blockNumber() + 1;
    int x = cursor.columnNumber() + 1;
    data += " Ln: " + QString::number(y) + " Col: " + QString::number(x);
    emit info(data);
}

void editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect content = contentsRect();
    m_editorPaintArea->setGeometry(QRect(content.left(), content.top(),
                                         linesNumberWidth(), content.height()));
}
