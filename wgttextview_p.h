#ifndef WGTTEXTVIEW_P
#define WGTTEXTVIEW_P

#include "wgttextview.h"
class QMenu;
class QToolBar;
class QAction;
class QPlainTextEdit;

class wgtTextViewPrivate
{
    Q_DECLARE_PUBLIC(wgtTextView);

public:
    wgtTextViewPrivate();
    ~wgtTextViewPrivate();

    QPlainTextEdit *textView;
    QString currentFile;

    wgtTextView *q_ptr;
};


#endif // WGTTEXTVIEW_P

