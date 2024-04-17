#include "exif_data_document_files.h"

exif_data_document_files::exif_data_document_files(QObject *parent) : QObject(parent)
{

}

exif_data_document_files::~exif_data_document_files()
{

}

QStringList exif_data_document_files::pub_get_all_available_exif_data_documents(QString filepath)
{
    return extract_all_available_exif_data_pdf_files(filepath);
}

QString exif_data_document_files::pub_get_exif_data_documents(QString filepath , struct_global_exif_data_parsed_data &exif_data_parsed_obj)
{
   return extract_exif_data_pdf_files(filepath , exif_data_parsed_obj);
}

QString exif_data_document_files::convert_numeric_timestamp_to_readable_for_display(QString epch_str)
{
    QString display_str;

    if(epch_str.isEmpty())
        return display_str;

    display_str = global_recon_helper_singular_class_obj->convert_numeric_to_readable_for_tablewidget(epch_str , Q_FUNC_INFO);

    return display_str;
}
