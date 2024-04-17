#ifndef RAM_ANALYSIS_SAVED_LOADER_H
#define RAM_ANALYSIS_SAVED_LOADER_H

#include <QObject>
#include "tablewidget_result_loader/table_result_loader.h"
#include "recon_generic_functions/recon_static_functions.h"

class ram_analysis_saved_loader : public table_result_loader
{
public:
    explicit ram_analysis_saved_loader(QWidget *parent);
    ~ram_analysis_saved_loader();


    void pub_set_ram_analysis_saved_display_db_path(QString db_path);
    void populate_ram_analysis_saved_data_in_table();


private slots:
    void slot_add_notes_pushbutton_clicked(QString text);

private:

    QString destination_ram_analysis_saved_db_path;

    void contextMenuEvent(QContextMenuEvent *evt);

    void tablewidget_general_clicked(int row, int column);

    void action_remove_note_triggered();
    void action_remove_note_bookmark_triggered();

    void update_bookmark_value(QString status, int row, int column);
    void update_notes_value(QString text, int current_row);
    void update_tags_value(QString tag_data, QString colour);
};

#endif // RAM_ANALYSIS_SAVED_LOADER_H
