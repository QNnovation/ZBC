/*
 * Created by Melnyk Anton 20.08.2015
 * class zbcMainWindow for Ze Best Commender(ZBC)
*/

//#include "zbccentralwidget.h"
#include "zbc_centralwidget.h"
#include "zbcmainwindow.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>


//C-tor
zbcMainWindow::zbcMainWindow(QWidget *pwgt) : QMainWindow(pwgt)
{
    createActions();
    setupMenuBar();
    setupToolBar();
//    setupCentralWidget();
    m_pzbcCwgt = new    ZBC_CentralWidget(this);
    this->setCentralWidget(m_pzbcCwgt);
}


//D-tor
zbcMainWindow::~zbcMainWindow(){}


//Actions
void zbcMainWindow::createActions()
{
    m_pactQuit          = new QAction(tr("Quit"), this);
    connect(m_pactQuit, &QAction::triggered,
            this, &QMainWindow::close);
}


//Menu
void zbcMainWindow::setupMenuBar()
{
    m_pmnuFile = menuBar()->addMenu(tr("File"));
    m_pmnuFile->addAction(m_pactQuit);
}


//ToolBar
void zbcMainWindow::setupToolBar()
{
    m_ptbrFile = addToolBar(tr("File"));
    m_ptbrFile->addAction(m_pactQuit);
}


//Central Widget
/*
void zbcMainWindow::setupCentralWidget()
{
    m_pzbcCWgt          = new zbcCentralWidget(this);
    this->setCentralWidget(m_pzbcCWgt);
}
*/
