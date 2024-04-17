#ifndef REDEFINED_RESULT_FILTERS_ADD_NEW_FILTER_H
#define REDEFINED_RESULT_FILTERS_ADD_NEW_FILTER_H

#include <QDialog>
#include<QtCore>
#include "recon_generic_functions/recon_static_functions.h"

namespace Ui {
class redefined_result_filters_add_new_filter;
}

class redefined_result_filters_add_new_filter : public QDialog
{
    Q_OBJECT

public:
    explicit redefined_result_filters_add_new_filter(QWidget *parent = 0);
    ~redefined_result_filters_add_new_filter();
    void refresh_filters_list_on_add_new_filter(QStringList filters_list);

signals:
    void signal_add_new_filter_in_tablewidget(QString);

private slots:   
    void on_pushButton_save_clicked();
    void on_lineEdit_filter_name_textChanged(const QString &arg1);

private:
    Ui::redefined_result_filters_add_new_filter *ui;

    QStringList redefined_result_filters_list;
};

#endif // REDEFINED_RESULT_FILTERS_ADD_NEW_FILTER_H
