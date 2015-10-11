#include "zbc_mainwindow.h"
#include <QApplication>
#include <QScopedPointer>

int main(int argc, char** argv)
{
    QApplication        zbcApp(argc, argv);
    QCoreApplication::setOrganizationName("QNnovation");
    QCoreApplication::setApplicationName("ZBC");
    QScopedPointer<ZBC_MainWindow> wnd( new ZBC_MainWindow );

    wnd->setWindowTitle("ZBC");
    wnd->show();
    return zbcApp.exec();
}
