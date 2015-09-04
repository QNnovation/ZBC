#include "zbcmainwindow.h"
#include <QApplication>
#include <QScopedPointer>

int main(int argc, char** argv)
{
    QApplication        zbcApp(argc, argv);
    QScopedPointer<zbcMainWindow> wnd( new zbcMainWindow );

    wnd->setWindowTitle("ZBC_02");
    wnd->showMaximized();
    return zbcApp.exec();
}