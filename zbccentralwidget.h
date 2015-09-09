#ifndef ZBCCENTRALWIDGET_H
#define ZBCCENTRALWIDGET_H

#include<QFrame>

class zbcSideFrame;
class zbcPushButton;
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
    zbcSideFrame*       m_psfwLeft;
    zbcSideFrame*       m_psfwRight;
    zbcSideFrame*       m_psfwActive;
    zbcSideFrame*       m_psfwNotActive;
    QSplitter*          m_psplCentral;

    QFrame*             m_pfrmBottomButtons;
    zbcPushButton*      m_pbtnView;
    zbcPushButton*      m_pbtnEdit;
    zbcPushButton*      m_pbtnCopy;
    zbcPushButton*      m_pbtnMove;
    zbcPushButton*      m_pbtnNewFolder;
    zbcPushButton*      m_pbtnDelete;
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
    void setActiveFrame(zbcSideFrame*);
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
