#include "htmltopdf.h"

htmltopdf::htmltopdf(QObject *parent)
{
    bool_cancel_pdf_report = false;

    //    htmltopdf_webengine_obj = new htmltopdf_webengine(this);
    //    connect(this,SIGNAL(signal_create_pdf_webengine(QString,QString)),htmltopdf_webengine_obj,SLOT(slot_create_pdf_webengine(QString,QString)));
    //    connect(htmltopdf_webengine_obj,SIGNAL(signal_htmltopdf_finished_webengine(QString,bool)),this,SLOT(slot_html_to_pdf_finished(QString,bool)));

    htmltopdf_wkhtmltopdf_obj = new htmltopdf_wkhtmltopdf(this);
    connect(this,SIGNAL(signal_create_pdf_wkhtmltopdf(QString,QString)),htmltopdf_wkhtmltopdf_obj,SLOT(slot_create_pdf_wkhtmltopdf(QString,QString)));
    connect(htmltopdf_wkhtmltopdf_obj,SIGNAL(signal_htmltopdf_finished_wkhtmltopdf(QString,bool)),this,SLOT(slot_html_to_pdf_finished(QString,bool)));
}

void htmltopdf::slot_create_pdf(QString source_html_file, QString dest_pdf_file)
{
    if(bool_cancel_pdf_report)
        return;


    //    qint64 file_size = 10000000; // 10 MB

    //    // PDF Report not created well if file size more than 10MB with webengine.
    //    // For large file size we used wkhtmltopdf executable.
    //    // wkhtmltopdf - image link not worked well.


    //    if(QFileInfo(source_html_file).size() < file_size)
    //    {
    //        emit signal_create_pdf_webengine(source_html_file,dest_pdf_file);
    //    }
    //    else
    //    {
    //        emit signal_create_pdf_wkhtmltopdf(source_html_file,dest_pdf_file);
    //    }

    emit signal_create_pdf_wkhtmltopdf(source_html_file,dest_pdf_file);

}


void htmltopdf::slot_html_to_pdf_finished(QString pdf_file_path, bool m_status)
{
    emit signal_html_to_pdf_finished(pdf_file_path,m_status);
}

void htmltopdf::pub_cancel_report()
{
    bool_cancel_pdf_report = true;

    htmltopdf_wkhtmltopdf_obj->pub_htmltopdf_cancelled_wkhtmltopdf();
}

void htmltopdf::set_essentials()
{
    bool_cancel_pdf_report = false;
}
