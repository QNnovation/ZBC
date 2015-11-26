#include "zbc_styles.h"
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
    QAction* pactQuit       = new QAction("Quit", this);

//Menu Bar
    QMenu* pmnuFile = menuBar()->addMenu("File");
    pmnuFile->addAction(pactQuit);

    QMenu* pmnuView = menuBar()->addMenu("View");
    QMenu* pmnuSetStyle     = new QMenu("Set Style", this);
    pmnuView->addMenu(pmnuSetStyle);
    for( QString styleName : QStyleFactory::keys() )
    {
        if (styleName == "Fusion"){
            QMenu* pmnuFusion   = new QMenu("Fusion");
            pmnuSetStyle->addMenu(pmnuFusion);
            QAction* pactFusionDefault  = new QAction("Default", this);
            QAction* pactFusionDark  = new QAction("Dark", this);
            pmnuFusion->addAction(pactFusionDefault);
            pmnuFusion->addAction(pactFusionDark);
            connect(pactFusionDefault,
                    &QAction::triggered,
                    [=](){
                    QApplication::setStyle(QStyleFactory::create("Fusion"));
            });
            connect(pactFusionDark,
                    &QAction::triggered,
                    [=](){
                    QApplication::setStyle(new ZBC_SimpleStyle);
            });
        }
        else{
            QAction* pactStyle  = new QAction(styleName, this);
            pmnuSetStyle->addAction(pactStyle);
            connect(pactStyle,
                    &QAction::triggered,
                    [=](){
                    QApplication::setStyle(QStyleFactory::create(styleName));
                    });
        }
    }


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
