#ifndef ONLINE_MAPS_DIALOG_H
#define ONLINE_MAPS_DIALOG_H

#include <QDialog>
#include <QWebEngineView>
#include <QVBoxLayout>

namespace Ui {
class online_maps_dialog;
}

class online_maps_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit online_maps_dialog(QWidget *parent = 0);
    ~online_maps_dialog();

    void pub_show_map(QString current_url_str);

    void closeEvent(QCloseEvent *);

    void pub_show_apple_maps(QString link_str);

signals:
    void signal_online_maps_closed();

private:
    Ui::online_maps_dialog *ui;

    QWebEngineView *view;

    QVBoxLayout *mainVbox;
};

#endif // ONLINE_MAPS_DIALOG_H
