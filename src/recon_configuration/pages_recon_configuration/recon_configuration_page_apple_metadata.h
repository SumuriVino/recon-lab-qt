#ifndef RECON_CONFIGURATION_PAGE_APPLE_METADATA_H
#define RECON_CONFIGURATION_PAGE_APPLE_METADATA_H

#include <QWidget>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_configuration_page_apple_metadata;
}

class recon_configuration_page_apple_metadata : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_apple_metadata(QWidget *parent = 0);
    ~recon_configuration_page_apple_metadata();

    void pub_set_caller_type(QString caller_type);

private slots:
    void on_checkBox_select_all_clicked(bool checked);

    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_tableWidget_apple_metadata_cellClicked(int row, int column);
    void slot_horizontal_header_section_clicked(int column);
private:
    Ui::recon_configuration_page_apple_metadata *ui;

    enum enum_page_apple_metadata
    {
        Page_Apple_Metadata_Bookmark_Display_Filter = 0,
        Page_Apple_Metadata_Bookmark_Metadata,
        Page_Apple_Metadata_Bookmark_Reporting,
        Page_Apple_Metadata_Title,
        Page_Apple_Metadata_Attribute_Name,
        Page_Apple_Metadata_Description
    };

    QString destination_db_file;
    recon_helper_standard *recon_helper_standard_obj;

    void prepare_apple_metadata_display();
};

#endif // RECON_CONFIGURATION_PAGE_APPLE_METADATA_H
