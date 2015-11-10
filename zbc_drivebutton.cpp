#include <QDebug>

#include "zbc_drivebutton.h"

#include <QPainter>
#include <QBrush>
#include <QColor>


//C-tor
ZBC_DriveButton::ZBC_DriveButton(const QString& sPath, QWidget* pwgt) : QPushButton(sPath, pwgt)
{
    QPalette    palette = this->palette();
    palette.setColor(QPalette::Button, Qt::red);
    this->autoFillBackground();
    this->setPalette(palette);
    this->update();
//    QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
//    gradient.setColorAt(0, Qt::red);
//    gradient.setColorAt(1, Qt::green);
//    QColor color(gradient);
//    QBrush brush(gradient);
//    QColor color(brush);
//    pltBackground.setBrush(QPalette::Base, brush );
//    pltBackground.setColor(QPalette::Window, Qt::red );
//    this->setPalette(pltBackground);
    qDebug() << "There";
}

/*
void ZBC_DriveButton::paintEvent(QPaintEvent *pe)
{
//    QRectF ellipseRect(width()*0.25, height()*0.25, width()*0.5, height()*0.5);

    QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
    gradient.setColorAt(0, Qt::red);
    gradient.setColorAt(1, Qt::green);

    QPainter painter;
    painter.begin(this);
    painter.fillRect(rect(), QBrush(gradient));
    painter.setBrush(QBrush(gradient));
    painter.end();

    QPushButton::paintEvent(pe);
}
*/
