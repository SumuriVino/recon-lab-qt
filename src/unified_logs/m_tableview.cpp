#include "m_tableview.h"

m_tableview::m_tableview()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setAlternatingRowColors(true);
    setShowGrid(false);
    installEventFilter(this);
}

void m_tableview::keyPressEvent(QKeyEvent *event)
{
    qint64 currentRow = currentIndex().row();
    qint64 rowCount = this->model()->rowCount();
    switch(event->key())
    {
    case Qt::Key_Up:
        if(currentRow <= 0)
            break;

        for(int count = currentRow; count >= 0 ; count--)
        {
            if(!isRowHidden(count))
            {
                int pre_row = get_previous_visible_row(count);
                if(pre_row >= 0)
                    selectRow(pre_row);
                break;
            }
        }

        this->scrollTo(this->currentIndex());
        emit signal_arrow_key_pressed();
        break;

    case Qt::Key_Down:
        if(currentRow == rowCount -1 )
            break;

        for(int count = currentRow; count < rowCount; count++)
        {
            if(!isRowHidden(count))
            {
                int next_row = get_next_visible_row(count);
                if(next_row >= 0)
                    selectRow(next_row);

                break;
            }
        }


        this->scrollTo(this->currentIndex());
        emit signal_arrow_key_pressed();
        break;


    default:
        QTableView::keyPressEvent(event);

    }

}
int m_tableview::get_previous_visible_row(int row_index)
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

int m_tableview::get_next_visible_row(int row_index)
{
    qint64 rowCount = this->model()->rowCount();

    int next_row = -1;
    row_index += 1;
    for(int count = row_index; count < rowCount; count++)
    {
        if(!isRowHidden(count))
        {
            next_row = count;
            break;
        }
    }

    return next_row;
}
