#ifndef CASE_INFO_DETAILS_H
#define CASE_INFO_DETAILS_H

#include <QtCore>
#include <QtSql>
#include <QWidget>

#include "header_plugin_extraction_interface.h"
#include "recon_generic_functions/recon_static_functions.h"

#include "header_global_variables_3.h"                         
#include "recon_generic_functions/recon_helper_standard.h"      
#include "header_global_variables_2.h"
#include "header_global_variables_4.h"

class case_info_details : public QWidget
{
public:
    case_info_details(QWidget *parent = 0);


    void pub_write_case_info_in_textfile_and_db();

    void pub_extract_full_caseinfo();


private:


    void extract_details_for_case_info_from_db();
    void extract_case_info_from_textfile();

    void write_case_info_in_database();
    void write_case_info_in_textfile();

    recon_helper_standard *recon_helper_standard_obj;

};

#endif // CASE_INFO_DETAILS_H
