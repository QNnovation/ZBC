#include <QDebug>

#include "zbc_drivebutton.h"

#include <QPainter>
#include <QBrush>
#include <QColor>


//C-tor
ZBC_DriveButton::ZBC_DriveButton(const QString& sPath, QWidget* pwgt) : QPushButton(sPath, pwgt)
{
/*
    QPalette    palette = this->palette();
    palette.setColor(QPalette::ButtonText , Qt::red);
    palette.setColor(QPalette::Button , Qt::green);
    palette.setColor(QPalette::Window , Qt::yellow);
    parentWidget()->setAutoFillBackground(true);
    parentWidget()->setPalette(palette);

    qDebug() << "There";
//    QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
//    gradient.setColorAt(0, Qt::red);
//    gradient.setColorAt(1, Qt::green);
//    QColor color(gradient);
//    QBrush brush(gradient);
//    QColor color(brush);
//    pltBackground.setBrush(QPalette::Base, brush );
//    pltBackground.setColor(QPalette::Window, Qt::red );
//    this->setPalette(pltBackground);
*/
}

/*
void ZBC_DriveButton::paintEvent(QPaintEvent *pe)
{


    QPushButton::paintEvent(pe);
}
*/
