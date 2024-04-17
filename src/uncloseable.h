#ifndef UNCLOSEABLE_H
#define UNCLOSEABLE_H

#include <QDialog>

namespace Ui {
class uncloseable;
}

class uncloseable : public QDialog
{
    Q_OBJECT

public:
    explicit uncloseable(QWidget *parent = nullptr);
    ~uncloseable();

    void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_quit_clicked();

private:
    Ui::uncloseable *ui;
};

#endif // UNCLOSEABLE_H
