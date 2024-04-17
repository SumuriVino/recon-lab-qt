#ifndef PAGE_CASE_INFO_H
#define PAGE_CASE_INFO_H

#include <QWidget>
#include <QtCore>

                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class page_case_info;
}

class page_case_info : public QWidget
{
    Q_OBJECT

public:
    explicit page_case_info(QWidget *parent = 0);
    ~page_case_info();

    void pub_set_case_info();
    void pub_set_initialise_case_info();

signals:
    void signal_case_info_essentials_filled(QString info);

private slots:

    void on_lineEdit_case_no_textChanged(const QString &arg1);

    void on_lineEdit_case_name_textChanged(const QString &arg1);

    void on_lineEdit_examiner_textChanged(const QString &arg1);

private:
    Ui::page_case_info *ui;
    void display_case_details_data();

    void update_case_info_in_db();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // PAGE_CASE_INFO_H
