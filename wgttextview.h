#ifndef WGTTEXTVIEW_H
#define WGTTEXTVIEW_H

#include <QMainWindow>

class wgtTextViewPrivate;
class wgtTextView : public QMainWindow
{
    Q_OBJECT

public:
    explicit wgtTextView(QWidget *parent = 0);
    ~wgtTextView();
    void loadFile(const QString&);

protected:
    QScopedPointer<wgtTextViewPrivate> const d_ptr;
    wgtTextView(wgtTextViewPrivate &dd, QWidget *parent);

private:
    Q_DECLARE_PRIVATE(wgtTextView);
};

#endif // WGTTEXTVIEW_H
