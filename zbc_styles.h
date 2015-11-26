#ifndef ZBC_SIMPLESTYLE_H
#define ZBC_SIMPLESTYLE_H

#include <QProxyStyle>

QT_BEGIN_NAMESPACE
class QPalette;
QT_END_NAMESPACE


class ZBC_SimpleStyle : public QProxyStyle
{
    Q_OBJECT

public:
    ZBC_SimpleStyle();

    void polish(QPalette& pal)Q_DECL_OVERRIDE;
};

#endif // ZBC_SIMPLESTYLE_H
