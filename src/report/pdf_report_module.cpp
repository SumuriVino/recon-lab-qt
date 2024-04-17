#include "pdf_report_module.h"

pdf_report_module::pdf_report_module(QObject *parent)
{
    html_report_module_obj = new html_standard_report_module(this);
    htmltopdf_obj = new htmltopdf(this);
    connect(htmltopdf_obj,SIGNAL(signal_html_to_pdf_finished(QString,bool)),this,SLOT(slot_htmltopdf_finished(QString,bool)));
    connect(this,SIGNAL(signal_create_pdf(QString,QString)),htmltopdf_obj,SLOT(slot_create_pdf(QString,QString)));
    bool_cancel_report = false;

}

void pdf_report_module::pub_cancel_report()
{
    bool_cancel_report = true;
    html_report_module_obj->pub_cancel_report();
    htmltopdf_obj->pub_cancel_report();
}

void pdf_report_module::pub_convert_html_to_pdf(QString html_file_path)
{
    if(!QFile::exists(html_file_path))
    {
        return;
    }

    QString pdf_filepath = html_file_path;
    if(pdf_filepath.endsWith(".html"))
    {
        pdf_filepath.chop(4); // html
        pdf_filepath.append("pdf");
    }

    emit signal_create_pdf(html_file_path, pdf_filepath);

}

void pdf_report_module::set_essentials()
{
    bool_cancel_report = false;

    html_report_module_obj->set_essentials();

    html_report_module_obj->pub_set_bool_for_pdf_report();

    htmltopdf_obj->set_essentials();
}

void pdf_report_module::set_narad_muni_for_report(narad_muni *mobj)
{
    html_report_module_obj->set_narad_muni_for_report(mobj);
}


void pdf_report_module::clear_exportfilelist_table()
{
    html_report_module_obj->clear_exportfilelist_table();
}

void pdf_report_module::create_full_report_pdf(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{

    html_report_module_obj->create_full_report_standard_html(file_path,selected_tablist, tabs_export_status_list, selected_tags_list);

}

void pdf_report_module::create_tag_report_pdf(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list)
{

    html_report_module_obj->create_tag_report_standard_html(file_path,selected_tablist, tabs_export_status_list,selected_tags_list);

}

void pdf_report_module::create_screen_report_pdf(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list)
{

    html_report_module_obj->create_screen_report_standard_html(file_path,selected_tablist, tabs_export_status_list);

}

void pdf_report_module::slot_htmltopdf_finished(QString pdf_file_path, bool m_status)
{
    emit signal_pdf_finished(pdf_file_path, m_status);
}
