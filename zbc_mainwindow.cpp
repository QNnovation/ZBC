#include <QDebug>

#include "zbc_mainwindow.h"
#include "zbc_centralwidget.h"

#include <QApplication>
#include <QMenuBar>
#include <QPalette>
#include <QToolBar>
#include <QSettings>
#include <QStyleFactory>


//C-tor
ZBC_MainWindow::ZBC_MainWindow(QWidget* pwgt) : QMainWindow(pwgt)
{
//Actions
    QAction* pactQuit   = new QAction("Quit", this);

//Menu Bar
    QMenu* pmnuFile = menuBar()->addMenu("File");
    pmnuFile->addAction(pactQuit);

//Tool Bar
    QToolBar* ptbrFile = addToolBar("File");
    ptbrFile->addAction(pactQuit);

//Connections
    connect(pactQuit,
            &QAction::triggered,
            this,
            &QMainWindow::close);

//Central Widget
    ZBC_CentralWidget* pzbcCwgt = new ZBC_CentralWidget(this);
    this->setCentralWidget(pzbcCwgt);

//Settings
    QSettings Settings;
    Settings.beginGroup("/Settings");
    Settings.beginGroup("/MainWindow");
    if (Settings.contains("/Geometry"))
        this->restoreGeometry(Settings.value("/Geometry").toByteArray());
    Settings.endGroup();
    Settings.endGroup();

//Debug BEGIN
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(0,102,0));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    QApplication::setPalette(darkPalette);
    qDebug() << QStyleFactory::keys();
//Debug END
}


//Override closeEvent
void ZBC_MainWindow::closeEvent(QCloseEvent *pe)
{
    QSettings Settings;
    Settings.beginGroup("/Settings");
    Settings.beginGroup("/MainWindow");
    Settings.setValue("Geometry", saveGeometry());
    Settings.endGroup();
    Settings.endGroup();

    QMainWindow::closeEvent(pe);
}
