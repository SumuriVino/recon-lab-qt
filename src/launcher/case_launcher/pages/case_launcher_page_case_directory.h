#ifndef CASE_DIRECTORY_H
#define CASE_DIRECTORY_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class page_case_directory;
}

class page_case_directory : public QWidget
{
    Q_OBJECT

public:
    explicit page_case_directory(QWidget *parent = 0);
    ~page_case_directory();

    void pub_set_output_dir_path();

    void pub_set_stored_output_path();
    bool pub_check_output_path_exist();
signals:
    void signal_result_dir_path_selected(QString);

private slots:
    void on_pushButton_case_dir_path_clicked();

private:
    Ui::page_case_directory *ui;
    QString output_dir_path;

    QFileDialog file_dialog_obj;
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // CASE_DIRECTORY_H
