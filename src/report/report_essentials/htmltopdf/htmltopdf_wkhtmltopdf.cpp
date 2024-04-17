#include "htmltopdf_wkhtmltopdf.h"


htmltopdf_wkhtmltopdf::htmltopdf_wkhtmltopdf(QObject *parent)
{
    recon_helper_process_obj = new recon_helper_process(this);
}

void htmltopdf_wkhtmltopdf::pub_htmltopdf_cancelled_wkhtmltopdf()
{
    recon_helper_process_obj->pub_cancel_process();
}

void htmltopdf_wkhtmltopdf::slot_create_pdf_wkhtmltopdf(QString source_html_file, QString dest_pdf_file)
{
    if(!QFile::exists(source_html_file) || !source_html_file.endsWith("html"))
    {
        emit signal_htmltopdf_finished_wkhtmltopdf(dest_pdf_file, false);
        return;
    }


    QString cmd = "../ZAlreadySigned/htmltopdf/wkhtmltopdf";

    QStringList arg_list;

    arg_list <<"--enable-local-file-access" << "--keep-relative-links" << source_html_file << dest_pdf_file;


    recon_helper_process_obj->run_command_with_arguments(cmd,arg_list,Q_FUNC_INFO);

    emit signal_htmltopdf_finished_wkhtmltopdf(dest_pdf_file,true);
}
