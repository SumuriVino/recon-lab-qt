#include "gallery_viewer.h"
#include "ui_gallery_viewer.h"

gallery_viewer::gallery_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gallery_viewer)
{
    ui->setupUi(this);

    ui->tableWidget_gallery->setColumnCount(8);

    for(int i = 0; i < ui->tableWidget_gallery->columnCount(); i++)
        ui->tableWidget_gallery->setColumnWidth(i, 150);

    bar = ui->tableWidget_gallery->verticalScrollBar();

    connect(bar, SIGNAL(valueChanged(int)), this, SLOT(slot_scroll_bar_value_changed(int)), Qt::QueuedConnection);

    // QThreadPool::globalInstance()->setMaxThreadCount(40);
    //connect(bar, SIGNAL(sliderReleased()), this, SLOT(slot_scroll_bar_slider_released()));
}

gallery_viewer::~gallery_viewer()
{
    delete ui;
}

void gallery_viewer::slot_scroll_bar_slider_released()
{
    int val = bar->value();

    slot_scroll_bar_value_changed(val);
}

void gallery_viewer::slot_bookmark_value_changed(struct_global_gallery_view_file_info f1)
{
    emit signal_gallery_bookmark_value_changed(f1);

    file_list->removeAt(f1.index);

    file_list->insert(f1.index, f1);
}

void gallery_viewer::slot_scroll_bar_value_changed(int val)
{

    int row_number = 0;

    if(val > 0)
        row_number = val / 150;

    int tablewidget_height = ui->tableWidget_gallery->height();
    int row_showing_currently = tablewidget_height / 150;


    QList<int> new_list_added;

    for(int jj = row_number; jj < row_number + row_showing_currently + 1; jj++)
    {

        for(int m_col = 0 ; m_col < 8; m_col++)
        {

            if(ui->tableWidget_gallery->cellWidget(jj, m_col) == NULL)
            {

                if(file_list->size() <= (jj*8 + m_col))
                    continue;

                gallery_item_cell *c1 = new gallery_item_cell;
                c1->pub_set_file_info(file_list->at(jj*8 + m_col));
                connect(c1, SIGNAL(signal_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_bookmark_value_changed(struct_global_gallery_view_file_info)));
                connect(c1, SIGNAL(signal_spacebar_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info)));
                connect(c1, SIGNAL(signal_control_e_key_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)));
                connect(c1, SIGNAL(signal_control_m_key_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_control_m_key_pressed(struct_global_gallery_view_file_info)));
                connect(c1, SIGNAL(signal_control_n_key_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_control_n_key_pressed(struct_global_gallery_view_file_info)));

                ui->tableWidget_gallery->setCellWidget(jj, m_col, c1);
                c1->pub_load_file_data();
            }

        }


        new_list_added << jj;

        for(int _m = 0; _m < list_of_loaded_rows.size(); _m++)
        {

            if(list_of_loaded_rows.at(_m) == jj)
            {
                list_of_loaded_rows.removeAt(_m);
                break;
            }
        }
    }

    for(int k = 0; k < list_of_loaded_rows.size(); k++)
    {
        for(int col =0 ; col < 8; col++)
        {
            ui->tableWidget_gallery->removeCellWidget(list_of_loaded_rows.at(k), col);
        }
    }

    list_of_loaded_rows.clear();
    list_of_loaded_rows = new_list_added;



    return;

    //    QThread* thread = new QThread;
    //    gallery_viewer_table_data_loader* worker = new gallery_viewer_table_data_loader();
    //    worker->pub_set_row_and_row_to_show(row_number,row_showing_currently );
    //    worker->pub_set_tablewidget(ui->tableWidget_gallery);
    //    worker->moveToThread(thread);
    //    connect(thread, SIGNAL (started()), worker, SLOT (load_data()));
    //    connect(worker, SIGNAL (finished()), thread, SLOT (quit()));
    //    connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
    //    connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
    //    thread->start();


    //    return;


}

void gallery_viewer::pub_set_file_list(QList<struct_global_gallery_view_file_info> *list)
{
    file_list = list;
}

void gallery_viewer::pub_create_and_fill_empty_items()
{

    QCoreApplication::processEvents();


    list_of_loaded_rows.clear();

    int minimum_rows_display = 8;

    if(file_list->size() / 8 < 8)
    {
        if(file_list->size() % 8 == 0)
            minimum_rows_display = file_list->size() / 8;
        else
            minimum_rows_display = (file_list->size() / 8) + 1;

        for(int m_row = 0; m_row < minimum_rows_display; m_row++)
        {
            ui->tableWidget_gallery->insertRow(m_row);
            ui->tableWidget_gallery->setRowHeight(m_row, 150);
        }
    }
    else
    {
        int total_row_count = 0;
        if(file_list->size() % 8 == 0)
            total_row_count = file_list->size() / 8;
        else
            total_row_count = (file_list->size() / 8) + 1;


        for(int m_row = 0; m_row < total_row_count; m_row++)
        {
            ui->tableWidget_gallery->insertRow(m_row);
            ui->tableWidget_gallery->setRowHeight(m_row, 150);
        }

    }



    int total_items_added = 0;
    for(int row = 0; row < minimum_rows_display; row++)
    {
        for(int col = 0 ; col < 8; col++)
        {
            if(total_items_added >= file_list->size())
                return;

            gallery_item_cell *c1 = new gallery_item_cell;
            c1->pub_set_file_info(file_list->at(total_items_added));
            connect(c1, SIGNAL(signal_bookmark_value_changed(struct_global_gallery_view_file_info)), this, SLOT(slot_bookmark_value_changed(struct_global_gallery_view_file_info)));
            connect(c1, SIGNAL(signal_spacebar_pressed(struct_global_gallery_view_file_info)), this, SIGNAL (signal_gallery_view_spacebar_pressed(struct_global_gallery_view_file_info)));
            connect(c1, SIGNAL(signal_control_e_key_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_control_e_key_pressed(struct_global_gallery_view_file_info)));
            connect(c1, SIGNAL(signal_control_m_key_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_control_m_key_pressed(struct_global_gallery_view_file_info)));
            connect(c1, SIGNAL(signal_control_n_key_pressed(struct_global_gallery_view_file_info)), this, SIGNAL(signal_gallery_view_control_n_key_pressed(struct_global_gallery_view_file_info)));

            c1->pub_load_file_data();
            ui->tableWidget_gallery->setCellWidget(row, col, c1);

            total_items_added++;

        }

    }





    return;
}


void gallery_viewer::pub_clear_gallery()
{
    ui->tableWidget_gallery->setRowCount(0);
}

void gallery_viewer::pub_set_message(QString str)
{
    ui->label_message->setText(str);
}

void gallery_viewer::pub_clear_selection()
{
    ui->tableWidget_gallery->clearSelection();
}

void gallery_viewer::on_tableWidget_gallery_cellClicked(int row, int column)
{
    gallery_item_cell *c1 = (gallery_item_cell *)ui->tableWidget_gallery->cellWidget(row, column);

    if(c1 == NULL)
    {
        ui->label_message->clear();
        struct_global_gallery_view_file_info tt;
        emit signal_gallery_viewer_item_clicked(tt);
        return;
    }

    ui->label_message->setText(c1->pub_file_path());

    emit signal_gallery_viewer_item_clicked(c1->pub_file_info());

}

void gallery_viewer::on_tableWidget_gallery_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    gallery_item_cell *c1 = (gallery_item_cell *)ui->tableWidget_gallery->cellWidget(currentRow, currentColumn);

    if(c1 == NULL)
    {
        ui->label_message->clear();
        struct_global_gallery_view_file_info tt;
        emit signal_gallery_viewer_item_clicked(tt);
        return;
    }

    ui->label_message->setText(c1->pub_file_path());

    emit signal_gallery_viewer_item_clicked(c1->pub_file_info());

}

void gallery_viewer::slot_set_bookmark_status_to_gallery_view(bool bookmark_status)
{
    gallery_item_cell *c1 = (gallery_item_cell *)ui->tableWidget_gallery->cellWidget(ui->tableWidget_gallery->currentRow(), ui->tableWidget_gallery->currentColumn());
    if(c1 == NULL)
        return;

    c1->pub_set_bookmark(bookmark_status);

}
