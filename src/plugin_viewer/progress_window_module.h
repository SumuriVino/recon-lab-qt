#ifndef PROGRESS_WINDOW_module_H
#define PROGRESS_WINDOW_module_H

#include <QDialog>

namespace Ui {
class progress_window_module;
}

class progress_window_module : public QDialog
{
    Q_OBJECT
    
public:
    explicit progress_window_module(const QString &title,QWidget *parent = 0);
    ~progress_window_module();
    void set_status_message(QString data);
    virtual void closeEvent(QCloseEvent *);
    
   void set_module_name(QString);

   void set_icon(QString icon_path);
signals:
    void signal_progress_window_module_closed();
private:
    Ui::progress_window_module *ui;
    QString w_title;
};

#endif // PROGRESS_WINDOW_H
