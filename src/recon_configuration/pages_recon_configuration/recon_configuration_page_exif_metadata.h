#ifndef RECON_CONFIGURATION_PAGE_EXIF_METADATA_H
#define RECON_CONFIGURATION_PAGE_EXIF_METADATA_H

#include <QWidget>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class recon_configuration_page_exif_metadata;
}

class recon_configuration_page_exif_metadata : public QWidget
{
    Q_OBJECT

public:
    explicit recon_configuration_page_exif_metadata(QWidget *parent = 0);
    ~recon_configuration_page_exif_metadata();


    void pub_set_caller_type(QString caller_type);

private slots:
    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_tableWidget_exif_metadata_cellClicked(int row, int column);
private:
    Ui::recon_configuration_page_exif_metadata *ui;

    enum enum_page_exif_metadata
    {
        Page_Exif_Metadata_Bookmark_Display_Filter = 0,
        Page_Exif_Metadata_Bookmark_Metadata,
        Page_Exif_Metadata_Bookmark_Reporting,
        Page_Exif_Metadata_Title,
        Page_Exif_Metadata_Attribute_Name,
        Page_Exif_Metadata_Description
    };

    QString destination_db_file;

    void prepare_exif_metadata_display();
    recon_helper_standard *recon_helper_standard_obj;

};

#endif // RECON_CONFIGURATION_PAGE_EXIF_METADATA_H
