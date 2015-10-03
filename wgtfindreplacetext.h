#ifndef FINDREPLACETEXT_H
#define FINDREPLACETEXT_H

#include <QDialog>
#include <QTextDocument>

class QDialogButtonBox ;
class QLineEdit;
class QLabel;
class QPushButton;
class QCheckBox;
class QRadioButton;

class FindReplaceText : public QDialog
{
    Q_OBJECT
public:
    explicit FindReplaceText(QWidget *parent = 0);
    void isReplace(bool);
    ~FindReplaceText();

public slots:
    void findSlot();
    void replaceSlot();
    void replaceAllSlot();
    void isEmptyText(QString);

signals:
    void findSignal(QString, QTextDocument::FindFlags);
    void replaceSignal(QString, QTextDocument::FindFlags, QString);
    void replaceAllSignal(QString, QTextDocument::FindFlags, QString);

private:
    //return flag;
    QTextDocument::FindFlags setOptions();
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

    //label status
    QLabel *statusLbl;

    //data
    QString newWord;
    QTextDocument::FindFlags flags;
};

#endif // FINDREPLACETEXT_H
