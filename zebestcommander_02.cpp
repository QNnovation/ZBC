#include "zbcmainwindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication        zbcApp(argc, argv);
    zbcMainWindow* wnd  = new zbcMainWindow;

    wnd->setWindowTitle("ZBC_02");
    wnd->showMaximized();
    return zbcApp.exec();
}
