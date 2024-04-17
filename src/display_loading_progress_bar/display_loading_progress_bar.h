#ifndef DISPLAY_LOADING_PROGRESS_BAR_H
#define DISPLAY_LOADING_PROGRESS_BAR_H

#include <QDialog>
#include <QtCore>
#include "recon_generic_functions/recon_static_functions.h"


namespace Ui {
class display_loading_progress_bar;
}


class display_loading_progress_bar : public QDialog
{
    Q_OBJECT

public:
    explicit display_loading_progress_bar(QWidget *parent = 0);
    ~display_loading_progress_bar();

    Ui::display_loading_progress_bar *ui;

    void closeEvent(QCloseEvent *close);

    void pub_set_label_messsge(QString label_text);

    void pub_set_progress_bar_definite_value(int value);
    void pub_set_progress_bar_indefinite_value();

    void pub_set_progress_bar_title(QString title_str);
    void pub_set_hidden_cancel_button(bool status);
    void pub_set_window_modal(Qt::WindowModality wdt);
signals:

    void signal_close_loading_display_dialog(bool status);

private slots:

    void on_pushButton_canel_clicked();

private:

};

#endif //
