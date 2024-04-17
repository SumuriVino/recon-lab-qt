#ifndef TIMELINE_module_H
#define TIMELINE_module_H

#include <QDialog>
#include <QtCore>

#include "message_box/message_dialog.h"

namespace Ui {
class timeline_selection_window;
}

class timeline_selection_window : public QDialog
{
    Q_OBJECT
    
public:
    explicit timeline_selection_window(QWidget *parent = 0);
    ~timeline_selection_window();
    
    void closeEvent(QCloseEvent *);

signals:
    void signal_time_line(qint64, qint64);

private slots:
    void on_pushButton_set_timeline_clicked();

private:
    Ui::timeline_selection_window *ui;
    QDateTime date_time;

    QDateTime end_date_time;

};

#endif // TIMELINE_H
