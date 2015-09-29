#ifndef ZBC_MAINWINDOW_H
#define ZBC_MAINWINDOW_H

#include <QMainWindow>

class ZBC_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ZBC_MainWindow(QWidget* pwgt = 0);

protected:
    virtual void closeEvent(QCloseEvent* pe) override;
};

#endif // ZBC_MAINWINDOW_H
