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

    bool rwMode;

    QMenu *menu;

    QAction *quitAct;


    void createMenu();
    void createActions();

    QPlainTextEdit *textView;
    QString pathToFile;

    void readFile();

    wgtTextView *q_ptr;
};


#endif // WGTTEXTVIEW_P

