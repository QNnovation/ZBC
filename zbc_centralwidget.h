#ifndef ZBC_CENTRALWIDGET_H
#define ZBC_CENTRALWIDGET_H

#include <QFrame>

class ZBC_CentralWidgetPrivate;

class ZBC_CentralWidget : public QFrame
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ZBC_CentralWidget)

    QScopedPointer<ZBC_CentralWidgetPrivate>    d_ptr;
public:
    explicit ZBC_CentralWidget(QWidget* pwgt = 0);
    virtual ~ZBC_CentralWidget();

protected:
    virtual void keyPressEvent(QKeyEvent* pe) override;

signals:
    void goBack();
//    void backAtEnd();
    void goForward();
};

#endif // ZBC_CENTRALWIDGET_H
