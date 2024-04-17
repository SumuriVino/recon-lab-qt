#ifndef PDF_REPORT_MODULE_H
#define PDF_REPORT_MODULE_H

#include <QWidget>
#include "export/export_for_reports/export_file.h"
#include "header_global_variables_3.h"
#include "header_global_variables_1.h"
#include "header_report.h"
#include "report/report_essentials/report_generic_function.h"
#include "report/html_standard_report_module.h"
#include "report/report_essentials/htmltopdf/htmltopdf.h"
#include "header_global_variables_2.h"

class pdf_report_module : public QObject
{
    Q_OBJECT
public:
    explicit pdf_report_module(QObject *parent = nullptr);

    void create_full_report_pdf(QString file_path,  QStringList selected_tablist, QStringList tabs_export_status_list,QStringList selected_tags_list);
    void create_tag_report_pdf( QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list, QStringList selected_tags_list);
    void create_screen_report_pdf(QString file_path, QStringList selected_tablist, QStringList tabs_export_status_list);

    void clear_exportfilelist_table(void);

    void set_essentials();
    void set_narad_muni_for_report(narad_muni *mobj);
    void pub_cancel_report();

    void pub_convert_html_to_pdf(QString html_file_path);

signals:

    void signal_create_pdf(QString source_html_file, QString dest_pdf_file);
    void signal_pdf_finished(QString pdf_file_path, bool m_status);

public slots:
    void slot_htmltopdf_finished(QString pdf_file_path, bool m_status);

private:

    html_standard_report_module *html_report_module_obj;
    htmltopdf *htmltopdf_obj;


    bool bool_cancel_report;

};

#endif // PDF_REPORT_MODULE_H
