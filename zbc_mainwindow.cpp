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
    QAction* pactQuit       = new QAction(QIcon(":/mainwindow/icons/resource/close.ico"), "Quit", this);
    QAction* pactBack       = new QAction(QIcon(":/buttons/toolbar/resource/left32.ico"), tr("Back"), this);
    QAction* pactForward    = new QAction(QIcon(":/buttons/toolbar/resource/right32.ico"), tr("Forward"), this);
    QAction* pactCMD        = new QAction(QIcon(":/buttons/toolbar/resource/cmd.ico"), tr("Run cmd.exe"), this);
    QAction* pactNotepad    = new QAction(QIcon(":/buttons/toolbar/resource/notepad.ico"), tr("Run notepad.exe"), this);
    QAction* pactPaint      = new QAction(QIcon(":/buttons/toolbar/resource/paint.ico"), tr("Run mspaint.exe"), this);
    QAction* pactCalc       = new QAction(QIcon(":/buttons/toolbar/resource/calc.ico"), tr("Run calc.exe"), this);
    QAction* pactTaskmgr    = new QAction(QIcon(":/buttons/toolbar/resource/taskmgr.ico"), tr("Run taskmgr.exe"), this);
//    QAction* pactRegedit    = new QAction(QIcon(":/buttons/toolbar/resource/regedit.ico"), tr("Run regedit.exe"), this);


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
/*
    QToolBar* ptbrFile = addToolBar("File");
    ptbrFile->setMovable(false);
    ptbrFile->addAction(pactQuit);
*/

    QToolBar* ptbrGo = addToolBar("Go");
    ptbrGo->setMovable(false);
    ptbrGo->addAction(pactBack);
    ptbrGo->addAction(pactForward);

    QToolBar* ptbrTools = addToolBar(tr("Tools"));
    ptbrTools->setMovable(false);
    ptbrTools->addAction(pactCMD);
    ptbrTools->addAction(pactNotepad);
    ptbrTools->addAction(pactPaint);
    ptbrTools->addAction(pactCalc);
    ptbrTools->addAction(pactTaskmgr);
//    ptbrTools->addAction(pactRegedit);

//Central Widget
    ZBC_CentralWidget* pzbcCwgt = new ZBC_CentralWidget(this);
    this->setCentralWidget(pzbcCwgt);


//Connections
//Quit
    connect(pactQuit,
            &QAction::triggered,
            this,
            &QMainWindow::close);

//Go Back
    connect(pactBack,
            &QAction::triggered,
            pzbcCwgt,
            &ZBC_CentralWidget::goBack
            );

//Go Forward
    connect(pactForward,
            &QAction::triggered,
            pzbcCwgt,
            &ZBC_CentralWidget::goForward);


//Run cmd.exe
    connect(pactCMD,
            &QAction::triggered,
            [=](){
                QProcess* pprcCmd   = new QProcess(this);
                pprcCmd->startDetached("cmd.exe");
            });

//Run notepad.exe
    connect(pactNotepad,
            &QAction::triggered,
            [=](){
                QProcess* pprcCmd   = new QProcess(this);
                pprcCmd->startDetached("notepad.exe");
            });

//Run mspaint.exe
        connect(pactPaint,
                &QAction::triggered,
                [=](){
                    QProcess* pprcCmd   = new QProcess(this);
                    pprcCmd->startDetached("mspaint.exe");
                });

//Run calc.exe
        connect(pactCalc,
                &QAction::triggered,
                [=](){
                    QProcess* pprcCmd   = new QProcess(this);
                    pprcCmd->startDetached("calc.exe");
                });

//Run taskmgr.exe
        connect(pactTaskmgr,
                &QAction::triggered,
                [=](){
                    QProcess* pprcCmd   = new QProcess(this);
                    pprcCmd->startDetached("taskmgr.exe");
                });

//Run regedit.exe !!! Doesn't work!!!
/*
        connect(pactRegedit,
                &QAction::triggered,
                [=](){
                    QProcess* pprcCmd   = new QProcess(this);
                    pprcCmd->startDetached("regedit.exe");
                    qDebug() << "regedit.exe";
                });
*/


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
