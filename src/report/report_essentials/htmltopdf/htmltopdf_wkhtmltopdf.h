#ifndef HTMLTOPDF_WKHTMLTOPDF_H
#define HTMLTOPDF_WKHTMLTOPDF_H

#include <QObject>
#include "recon_generic_functions/recon_helper_process.h"

class htmltopdf_wkhtmltopdf : public QWidget
{
    Q_OBJECT
public:
    explicit htmltopdf_wkhtmltopdf(QObject *parent = nullptr);
    void pub_htmltopdf_cancelled_wkhtmltopdf();

signals:
    void signal_htmltopdf_finished_wkhtmltopdf(QString pdf_file_path, bool m_status);

public slots:
    void slot_create_pdf_wkhtmltopdf(QString source_html_file, QString dest_pdf_file);

private:
    recon_helper_process *recon_helper_process_obj;

};

#endif // HTMLTOPDF_WKHTMLTOPDF_H
