#ifndef TEXT_VIEWER_H
#define TEXT_VIEWER_H

#include <QWidget>

#include "progress_bar/progress_bar_window.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_1.h"
#include "add_tags/add_tags.h"
#include "header_identical_2.h"
#include "header_global_variables_2.h"


#include "hexedit/qhexedit.h"
#include "recon_generic_functions/recon_helper_standard.h"      

namespace Ui {
class text_viewer;
}

class text_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit text_viewer(QWidget *parent = 0);
    ~text_viewer();

    void pub_load_file_in_text_view(QString filepath);
    void pub_set_essentials();


    void pub_clear_textview();
    void pub_set_source_count_name(QString str);
signals:
    void signal_insert_tag_detail_into_tag_search_db(QString file_path, QString tag_name, uint64_t start_index, uint64_t end_index, QString record_no, QString text_viewer_display_name, QString source_count_name);

private slots:


    void slot_add_tag_to_tag_search(qint64 start_offset, qint64 end_offset, QString tag_name, QString tag_color, QString color_hex, QByteArray tagged_content);
private:
    Ui::text_viewer *ui;

    QFile file;
    QHexEdit_textviewer *hexEdit;

    QString current_file_path;
    QString source_count_name;

    void loadFile(const QString &fileName);
    recon_helper_standard *recon_helper_standard_obj;

    void insert_exported_content_info_in_db(QString tag_name, QString color_hex, qint64 start_index, qint64 end_index, QByteArray file_contents_arr);
};

#endif // TEXT_VIEWER_H
