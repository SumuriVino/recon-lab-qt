#include "exif_data_image_files.h"

exif_data_image_files::exif_data_image_files(QWidget *parent): QWidget(parent)
{
}

exif_data_image_files::~exif_data_image_files()
{
}


QString exif_data_image_files::pub_get_exif_data_images(QString filepath , struct_global_exif_data_parsed_data &exif_data_parsed_obj)
{
    QString image_exif_with_xml = extract_exif_data_image_files(filepath , exif_data_parsed_obj);

   return image_exif_with_xml;

}

QStringList exif_data_image_files::pub_get_all_available_exif_data_images(QString filepath)
{
    QStringList image_exif_list = extract_all_available_exif_data_image_files(filepath);

    return image_exif_list;

}


