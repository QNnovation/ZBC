#include "wgttextview.h"
#include "wgttextview_p.h"

#include <QtWidgets>

wgtTextView::wgtTextView(QWidget *parent)
    :QMainWindow(parent)
    , d_ptr(new wgtTextViewPrivate())
{
    Q_D(wgtTextView);
    d->q_ptr = this;

    setCentralWidget(d->textView);
}

wgtTextView::~wgtTextView()
{

}

void wgtTextView::loadFile(const QString &)
{

}

wgtTextView::wgtTextView(wgtTextViewPrivate &dd, QWidget *parent)
    : QMainWindow(parent)
      , d_ptr(&dd)
{
    Q_D(wgtTextView);
    d->q_ptr = this;
}


wgtTextViewPrivate::wgtTextViewPrivate()
{
    textView = new QPlainTextEdit;
}

wgtTextViewPrivate::~wgtTextViewPrivate()
{

}
