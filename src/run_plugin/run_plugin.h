#ifndef RUN_PLUGIN_H
#define RUN_PLUGIN_H

#include <QWidget>
#include "launcher/plugin_selector/plugin_selector.h"
#include <QVBoxLayout>
                                   
#include <QtCore>
#include <QtGui>
#include <QSplitter>
#include <QPushButton>
namespace Ui {
class run_plugin;
}

class run_plugin : public QFrame
{
    Q_OBJECT

public:
    explicit run_plugin(QWidget *parent = 0);
    ~run_plugin();


    void start_work();
signals:

    void signal_plugin_list_from_run_plugin();

private slots:
    void on_pushButton_start_clicked();

private:
    Ui::run_plugin *ui;

    plugin_selector *plugin_selector_obj;
    QPushButton *pushbutton_start;

};

#endif // RUN_PLUGIN_H
