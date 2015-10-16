#include "zbc_mainwindow.h"
#include "zbc_centralwidget.h"

#include <QMenuBar>
#include <QToolBar>
#include <QSettings>


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
