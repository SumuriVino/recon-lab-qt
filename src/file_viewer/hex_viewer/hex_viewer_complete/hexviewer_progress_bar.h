#ifndef HEXVIEWER_PROGRESS_BAR_H
#define HEXVIEWER_PROGRESS_BAR_H

#include <QDialog>

namespace Ui {
class hexviewer_progress_bar;
}

class hexviewer_progress_bar : public QDialog
{
    Q_OBJECT

public:
    explicit hexviewer_progress_bar(QWidget *parent = 0);
    ~hexviewer_progress_bar();

private:
    Ui::hexviewer_progress_bar *ui;
};

#endif // HEXVIEWER_PROGRESS_BAR_H
