#ifndef CASE_INFO_DISPLAY_H
#define CASE_INFO_DISPLAY_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

#include "header_global_variables_3.h"                         
#include "header_identical_1.h"
#include "m_custom_source_button.h"
                                   
#include "header_global_structures.h"


namespace Ui {
class case_info_display;
}

class case_info_display : public QWidget
{
    Q_OBJECT

public:
    explicit case_info_display(QWidget *parent = 0);
    ~case_info_display();

    void pub_initialise_case_info_display();

private slots:
    void slot_toplevel_source_button_clicked(bool expand, int id, QString src_name, QString os_scheme);
    void slot_link_label_sub_source_list(QString link_txt);
private:
    Ui::case_info_display *ui;

    void display_sources_in_tree();

    QString get_device_detail_filled_in_source_textEdit(struct_GLOBAL_witness_info_source target_source_info);

};

#endif // CASE_INFO_DISPLAY_H
