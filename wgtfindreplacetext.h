#ifndef FINDREPLACETEXT_H
#define FINDREPLACETEXT_H

#include <QDialog>

class QDialogButtonBox ;
class QLineEdit;
class QLabel;
class QPushButton;
class QCheckBox;
class QRadioButton;

class FindReplaceText : public QDialog
{
public:
    explicit FindReplaceText(QWidget *parent = 0);
    ~FindReplaceText();

private:
    //line edit group
    QLineEdit *textFindEdit;
    QLineEdit *textReplaceEdit;
    QLabel *textFindLbl;
    QLabel *textReplaceLbl;

    //buttons
    QPushButton *findBtn;
    QPushButton *closeBtn;
    QPushButton *replaceBtn;
    QPushButton *replaceAllBtn;

    //radiobutton group
    QLabel *directionLbl;
    QRadioButton *downBtn;
    QRadioButton *upBtn;

    //checkbox group
    QLabel *optionsLbl;
    QCheckBox *caseSensitiveBox;
    QCheckBox *wholeWordsBox;
    QCheckBox *regularExprBox;

    //label status
    QLabel *statusLbl;
};

#endif // FINDREPLACETEXT_H
