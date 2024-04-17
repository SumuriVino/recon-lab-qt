#ifndef CUSTOM_TABLE_HEADER_H
#define CUSTOM_TABLE_HEADER_H

#include <QHeaderView>
#include <QtCore>

#include "custom_table_header/custom_table_header_widget.h"

class custom_table_header : public QHeaderView
{

public:
    struct Margins
    {
        int left;
        int right;
        int top;
        int buttom;

        Margins(int left = 2, int right = 2, int top = 2, int buttom = 2);
    };

    custom_table_header(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~custom_table_header();

    void set_scroll_positions();

    void set_item_widget(int index, QWidget * widget);
    void set_item_margins(int index, Margins margins);

private:
     struct Item
     {
         QWidget * item;
         Margins margins;

         Item();
     };

     QMap<int, Item> m_items;

     void showEvent(QShowEvent * e);
     void handle_section_resized(int i);
     void handle_section_moved(int logical, int oldVisualIndex, int newVisualIndex);
};
#endif // CUSTOM_TABLE_HEADER_H
