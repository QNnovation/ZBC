#ifndef ZBCCENTRALWIDGET_H
#define ZBCCENTRALWIDGET_H

#include<QFrame>

class ZBC_SideFrame;
class ZBC_PushButton;
class QSplitter;
class QPushButton;

class zbcCentralWidget : public QFrame
{
    Q_OBJECT

public:
    explicit zbcCentralWidget(QWidget *pwgt = 0);
    ~zbcCentralWidget();

private:
//MEMBERS
    ZBC_SideFrame*       m_psfwLeft;
    ZBC_SideFrame*       m_psfwRight;
    ZBC_SideFrame*       m_psfwActive;
    ZBC_SideFrame*       m_psfwNotActive;
    QSplitter*          m_psplCentral;

    QFrame*             m_pfrmBottomButtons;
    ZBC_PushButton*      m_pbtnView;
    ZBC_PushButton*      m_pbtnEdit;
    ZBC_PushButton*      m_pbtnCopy;
    ZBC_PushButton*      m_pbtnMove;
    ZBC_PushButton*      m_pbtnNewFolder;
    ZBC_PushButton*      m_pbtnDelete;
    QPushButton*        m_pbtnExit;

    QAction*            m_pactView;
    QAction*            m_pactEdit;
    QAction*            m_pactCopy;
    QAction*            m_pactMove;
    QAction*            m_pactNewFolder;
    QAction*            m_pactDelete;

//METHODS
    void createBottomFrame();
    void createView();
    void createConnections();
    void createActions();

private slots:
    void setActiveFrame(ZBC_SideFrame*);
    void runView();
    void runEdit();
    void runCopy();
    void runMove();
    void runNewFolder();
    void runDelete();

signals:
    void fileAdded(/*QString*/);

};

#endif // ZBCCENTRALWIDGET_H
