#include "m_tablewidget.h"


m_tablewidget::m_tablewidget(QWidget *parent) :
    QTableWidget(parent)
{
    //verticalScrollBar()->setSliderDown(true);

    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setGridStyle(Qt::NoPen);
    setShowGrid(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAutoScroll(true);
    QFont m_font = font();

    m_font.setPointSize(12);
    setFont(m_font);

    QString stylesheet = QString(

                //                "QTableView::item {"
                //                "border-top: 1px solid white;"
                //                " }"

                "QTableView::item:selected:!active {"
                "color:white;"
                "background-color:  #a1c9f7;"
                "}"

                "QTableView::indicator:checked {background-color:black;border: 1px solid #d6d6d6;}"
                "QTableView::indicator:unchecked {background-color:#f1f1f1;border: 1px solid #777777;}"

                "QTableView::indicator {"
                "width: 13px;"
                "height: 13px;}"
                );


    custom_horizontal_header = new custom_table_header(Qt::Orientation::Horizontal, this);
    setHorizontalHeader(custom_horizontal_header);
    horizontalHeader()->setMinimumHeight(65);

    setStyleSheet(stylesheet);


    id_num = 0;
    tab_name.clear();
}



void m_tablewidget::keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_Up:
        if(currentRow() <= 0)
            break;
        // setCurrentCell(currentRow() - 1, 1);

        for(int count = currentRow(); count >= 0 ; count--)
        {
            if(!isRowHidden(count))
            {
                //selectRow(count - 1);
                int pre_row = get_previous_visible_row(count);
                if(pre_row >= 0)
                    selectRow(pre_row);
                break;
            }
        }

        this->scrollToItem(this->currentItem());
        emit signal_arrow_key_pressed();
        break;

    case Qt::Key_Down:
        if(currentRow() == rowCount() -1 )
            break;

        //  setCurrentCell(currentRow() + 1, 1);

        for(int count = currentRow(); count < rowCount(); count++)
        {
            if(!isRowHidden(count))
            {
                //selectRow(count + 1);
                int next_row = get_next_visible_row(count);
                if(next_row >= 0)
                    selectRow(next_row);

                break;
            }
        }


        this->scrollToItem(this->currentItem());
        emit signal_arrow_key_pressed();
        break;


    case Qt::Key_Space:
        if(currentRow() < 0)
            break;

        emit signal_spacebar_quicklook_pressed();
        break;


    case Qt::Key_B:
        if(currentRow() < 0)
            break;
        //        for(int count = currentRow(); count < rowCount(); count++)
        //        {
        //            if(!isRowHidden(count))
        //            {
        //                selectRow(count);
        //                break;
        //            }
        //        }

        emit signal_bookmarked_key_pressed();
        break;

    case Qt::Key_M:
        if(currentRow() < 0)
            break;
        if(event->modifiers() & Qt::ControlModifier)
        {
            emit signal_control_m_key_pressed();
        }
        break;

    case Qt::Key_N:
        if(currentRow() < 0)
            break;
        if(event->modifiers() & Qt::ControlModifier)
        {
            emit signal_control_n_key_pressed();
        }
        break;

    case Qt::Key_E:
        if(currentRow() < 0)
            break;
        if(event->modifiers() & Qt::ControlModifier)
        {
            emit signal_control_e_key_pressed();
        }
        break;

    case Qt::Key_A:
        //        if(currentRow() < 0)
        //            break;
        //        if(event->modifiers() & Qt::ControlModifier)
        //        {
        //            emit signal_control_a_key_pressed();

        QTableWidget::keyPressEvent(event);

        //        }
        break;

    default:
        QTableWidget::keyPressEvent(event);

    }

}

QList<QTableWidgetItem*> m_tablewidget::takeRow(int row)
{
    QList<QTableWidgetItem*> rowItems;
    for (int col = 0; col < columnCount(); ++col)
    {
        rowItems << takeItem(row, col);
    }
    return rowItems;
}

// sets the whole row
void m_tablewidget::setRow(int row, const QList<QTableWidgetItem*>& rowItems)
{
    for (int col = 0; col < columnCount(); ++col)
    {
        setItem(row, col, rowItems.at(col));
    }
}

void m_tablewidget::pub_set_id(int id)
{
    id_num = id;
}

int m_tablewidget::pub_get_id()
{
    return id_num;
}

void m_tablewidget::pub_set_tab_name(QString tb_nm)
{
    tab_name = tb_nm;
}

QString m_tablewidget::pub_get_tab_name()
{
    return tab_name;
}

int m_tablewidget::get_previous_visible_row(int row_index)
{
    int previous_row = -1;
    row_index -= 1;
    for(int count = row_index; count >= 0 ; count--)
    {
        if(!isRowHidden(count))
        {
            previous_row = count;
            break;
        }
    }

    return previous_row;
}

int m_tablewidget::get_next_visible_row(int row_index)
{
    int next_row = -1;
    row_index += 1;
    for(int count = row_index; count < rowCount(); count++)
    {
        if(!isRowHidden(count))
        {
            next_row = count;
            break;
        }
    }

    return next_row;
}

void m_tablewidget::scrollContentsBy(int dx, int dy)
{
    QTableWidget::scrollContentsBy(dx, dy);

    if(custom_horizontal_header != NULL)
    {
        if (dx != 0)
        {
            custom_horizontal_header->set_scroll_positions();
        }
    }
}

void m_tablewidget::pub_set_horizontal_header_item_widget(int column, QWidget * widget)
{
    custom_horizontal_header->set_item_widget(column, widget);
}

void m_tablewidget::pub_set_horizontal_header_item_margins(int column, custom_table_header::Margins margins)
{
    custom_horizontal_header->set_item_margins(column, margins);
}
