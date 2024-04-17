#ifndef HTML_TO_PDF_H
#define HTML_TO_PDF_H

#include <QWidget>
//#include "htmltopdf_webengine.h"
#include "htmltopdf_wkhtmltopdf.h"


class htmltopdf: public QObject
{
    Q_OBJECT

public:
    explicit htmltopdf(QObject *parent = nullptr);

    void pub_cancel_report();
    void set_essentials();

signals:
    void signal_html_to_pdf_finished(QString pdf_file_path, bool m_status);
    void signal_create_pdf_webengine(QString source_html_file, QString dest_pdf_file);
    void signal_create_pdf_wkhtmltopdf(QString source_html_file, QString dest_pdf_file);


public slots:
    void slot_html_to_pdf_finished(QString pdf_file_path, bool m_status);
    void slot_create_pdf(QString source_html_file, QString dest_pdf_file);


private:
//    htmltopdf_webengine *htmltopdf_webengine_obj;
    htmltopdf_wkhtmltopdf *htmltopdf_wkhtmltopdf_obj;

    bool bool_cancel_pdf_report;

};

#endif // HTML_TO_PDF_H
