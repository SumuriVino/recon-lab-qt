#ifndef PROGRESS_BAR_FIXED_IN_STATUSBAR_H
#define PROGRESS_BAR_FIXED_IN_STATUSBAR_H

#include <QWidget>
#include <QBitmap>
#include <QtCore>

namespace Ui {
class progress_bar_fixed_in_statusbar;
}

class progress_bar_fixed_in_statusbar : public QWidget
{
    Q_OBJECT

public:
    explicit progress_bar_fixed_in_statusbar(QWidget *parent = 0);
    ~progress_bar_fixed_in_statusbar();

    void set_status_message(QString msg);
    //    void set_progressbar_size(qint64 height,qint64 width);
    void set_progressbar_size(QSize siz);
    void set_progressbar_value(int value);
    void set_record_count_string(QString count_str);
    void set_move_progressbar_in_left_corner(bool value);
    int get_progressbar_value();

signals:
    void signal_close_window();

private slots:
    void on_pushButton_close_clicked();

private:
    Ui::progress_bar_fixed_in_statusbar *ui;
};

#endif // PROGRESS_BAR_FIXED_IN_STATUSBAR_H
