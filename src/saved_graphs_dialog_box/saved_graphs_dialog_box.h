#ifndef SAVED_GRAPHS_DIALOG_BOX_H
#define SAVED_GRAPHS_DIALOG_BOX_H

#include <QDialog>

#include <QFrame>
#include <QtCore>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "previewer/image_preview.h"
#include <QFileDialog>

namespace Ui {
class saved_graphs_dialog_box;
}

class saved_graphs_dialog_box : public QDialog
{
    Q_OBJECT

public:
    explicit saved_graphs_dialog_box(QWidget *parent = 0);
    ~saved_graphs_dialog_box();

void pub_set_graph_name(QString graph_name) ;


void pub_clear_graph_name();
signals:
    void signal_save_graph_name(QString);

   // void signal_graph_work_complete();


private slots:
    void on_pushButton_clicked();

private:
    Ui::saved_graphs_dialog_box *ui;

    QString saved_graphs_dir;


};

#endif // SAVED_GRAPHS_DIALOG_BOX_H
