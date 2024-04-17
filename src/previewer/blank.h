#ifndef BLANK_H
#define BLANK_H

#include <QFrame>

namespace Ui {
class blank;
}

class blank : public QFrame
{
    Q_OBJECT
    
public:
    explicit blank(QWidget *parent = 0);
    ~blank();
    
    void pub_set_label_text(QString text);
private:
    Ui::blank *ui;
};

#endif // BLANK_H
