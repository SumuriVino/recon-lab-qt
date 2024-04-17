#ifndef PROGRESS_BAR_WINDOW_H
#define PROGRESS_BAR_WINDOW_H

#include <QDialog>

namespace Ui {
class progress_bar_window;
}

class progress_bar_window : public QDialog
{
    Q_OBJECT

public:
    explicit progress_bar_window(QWidget *parent = 0);
    ~progress_bar_window();
    void set_status_message(QString msg);
    void set_window_title(QString title);
    void closeEvent(QCloseEvent *);
    void set_application_modal();
    void set_window_modal();

    void set_icon(QString);

signals:
    void signal_close_window();

protected:
    void keyPressEvent(QKeyEvent *obj);
private:
    Ui::progress_bar_window *ui;
};

#endif
