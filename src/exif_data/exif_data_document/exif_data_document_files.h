#ifndef EXIF_DOCUMENT_FILES_H
#define EXIF_DOCUMENT_FILES_H

#include <QtCore>
#include <QObject>

#include "header_identical_1.h"
#include "header_global_variables_3.h"                         

class exif_data_document_files : public QObject
{
    Q_OBJECT
public:
    explicit exif_data_document_files(QObject *parent = 0);
    ~exif_data_document_files();

    QString pub_get_exif_data_documents(QString filepath, struct_global_exif_data_parsed_data &exif_data_parsed_obj);

    QStringList pub_get_all_available_exif_data_documents(QString filepath);

signals:

public slots:

private:

    QString extract_exif_data_pdf_files(QString filepath, struct_global_exif_data_parsed_data &exif_data_parsed_obj);
    QString convert_numeric_timestamp_to_readable_for_display(QString epch_str);
    QStringList extract_all_available_exif_data_pdf_files(QString filepath);

};

#endif // EXIF_DOCUMENT_FILES_H
