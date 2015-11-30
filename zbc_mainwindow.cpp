#include <QDebug>

#include "zbc_styles.h"
#include "zbc_mainwindow.h"
#include "zbc_centralwidget.h"

#include <QApplication>
#include <QDir>
#include <QMenuBar>
#include <QPalette>
#include <QProcess>
#include <QToolBar>
#include <QSettings>
#include <QStyleFactory>


//C-tor
ZBC_MainWindow::ZBC_MainWindow(QWidget* pwgt) : QMainWindow(pwgt)
{
    this->setWindowIcon(QIcon(":/mainwindow/icons/resource/app.ico"));

//Actions
    QAction* pactQuit       = new QAction("Quit", this);
    QAction* pactBack       = new QAction(QIcon(":/buttons/toolbar/resource/left32.ico"), tr("Back"), this);
    QAction* pactForward    = new QAction(QIcon(":/buttons/toolbar/resource/right32.ico"), tr("Forward"), this);
    QAction* pactCMD        = new QAction(QIcon(":/buttons/toolbar/resource/cmd.ico"), tr("Run cmd.exe"), this);


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
    ptbrFile->setMovable(false);
    ptbrFile->addAction(pactQuit);

    QToolBar* ptbrGo = addToolBar("Go");
    ptbrGo->setMovable(false);
    ptbrGo->addAction(pactBack);
    ptbrGo->addAction(pactForward);

    QToolBar* ptbrTools = addToolBar(tr("Tools"));
    ptbrTools->setMovable(false);
    ptbrTools->addAction(pactCMD);

//Connections
//Quit
    connect(pactQuit,
            &QAction::triggered,
            this,
            &QMainWindow::close);
//Run cmd.exe
    connect(pactCMD,
            &QAction::triggered,
            [=](){
                QProcess* pprcCmd   = new QProcess(this);
                pprcCmd->startDetached("cmd.exe");
            });


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
