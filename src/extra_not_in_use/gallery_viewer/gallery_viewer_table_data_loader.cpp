#include "gallery_viewer_table_data_loader.h"

gallery_viewer_table_data_loader::gallery_viewer_table_data_loader(QObject *parent) : QObject(parent)
{

}

void gallery_viewer_table_data_loader::pub_set_row_and_row_to_show(int m_row, int m_row_to_show)
{
    starting_row = m_row;
    row_to_show = m_row_to_show;
}

void gallery_viewer_table_data_loader::pub_set_tablewidget(QTableWidget *table)
{
    tablewidget = table;
}

void gallery_viewer_table_data_loader::load_data()
{
    for(int row = starting_row; row < starting_row + row_to_show + 1; row++)
    {
        for(int col = 0; col < 8; col++)
        {

            gallery_item_cell *c1 = (gallery_item_cell *)tablewidget->cellWidget(row, col);

            if(c1 == NULL)
                return;

            if(c1->pub_is_2file_data_loaded())
                continue;

            c1->pub_load_file_data();
        }

    }


    emit finished();
}
