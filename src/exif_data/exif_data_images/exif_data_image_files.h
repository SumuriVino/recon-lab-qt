#ifndef EXIF_DATA_IMAGE_FILES_H
#define EXIF_DATA_IMAGE_FILES_H

#include <QWidget>
#include <QtCore>

#include "recon_generic_functions/recon_static_functions.h"

class exif_data_image_files :  public QWidget
{
    Q_OBJECT

public:
    exif_data_image_files(QWidget *parent = 0);
    ~exif_data_image_files();

    QString pub_get_exif_data_images(QString filepath, struct_global_exif_data_parsed_data &exif_data_parsed_obj);

    QStringList pub_get_all_available_exif_data_images(QString filepath);
    QMap<QString, QString> pub_extract_specific_attributes(QString filepath, QStringList attr_list);
    QMap<QString, QString> pub_extract_latitude_longitude_attributes(QString filepath, QStringList attr_list);

private:

    QString extract_exif_data_image_files(QString file_path, struct_global_exif_data_parsed_data &exif_data_parsed_obj);

    void check_metadata_keys_exist_in_nsdictionary(void *ptr_nsdictionary);

    QStringList all_keys_list;

    QStringList extract_all_available_exif_data_image_files(QString file_path);
};

#endif // EXIF_DATA_IMAGE_FILES_H
