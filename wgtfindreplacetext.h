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
    void isEmptyText(const QString &);

signals:
    void findSignal(const QString &, QTextDocument::FindFlags);
    void replaceSignal(const QString &, QTextDocument::FindFlags, const QString &);
    void replaceAllSignal(const QString &, QTextDocument::FindFlags, const QString &);

private:
    //return flag;
    QTextDocument::FindFlags options() const;

    //line edit group
    QLineEdit    *m_textFindEdit;
    QLineEdit    *m_textReplaceEdit;
    QLabel       *m_textFindLbl;
    QLabel       *m_textReplaceLbl;

    //buttons
    QPushButton  *m_findBtn;
    QPushButton  *m_closeBtn;
    QPushButton  *m_replaceBtn;
    QPushButton  *m_replaceAllBtn;

    //radiobutton group
    QLabel       *m_directionLbl;
    QRadioButton *m_downBtn;
    QRadioButton *m_upBtn;

    //checkbox group
    QLabel       *m_optionsLbl;
    QCheckBox    *m_caseSensitiveBox;
    QCheckBox    *m_wholeWordsBox;

    //label status
    QLabel       *m_statusLbl;

    //data
    QString       m_newWord;
    QTextDocument::FindFlags flags;
};

#endif // FINDREPLACETEXT_H
