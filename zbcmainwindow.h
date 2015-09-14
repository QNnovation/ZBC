/*
 * Created by Melnyk Anton 20.08.2015
 * class zbcMainWindow for Ze Best Commender(ZBC)
*/

#ifndef ZBCMAINWINDOW_H
#define ZBCMAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;
class QToolBar;
//class zbcCentralWidget;
class ZBC_CentralWidget;

class zbcMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit zbcMainWindow(QWidget *pwgt = 0);
    ~zbcMainWindow();

private:
//MEMBERS
    QMenu*              m_pmnuFile;
    QToolBar*           m_ptbrFile;
    QAction*            m_pactQuit;
//    ZBC_CentralWidget   m_pzbcCWgt;

//METHODS
    void createActions();
    void setupMenuBar();
    void setupToolBar();
//    void setupCentralWidget();
};

#endif // ZBCMAINWINDOW_H
