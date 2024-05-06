#include "custom_table_header.h"

custom_table_header::custom_table_header(Qt::Orientation orientation, QWidget * parent) :
    QHeaderView(orientation, parent)
{
    connect(this, &custom_table_header::sectionResized, this, &custom_table_header::handle_section_resized);
    connect(this, &custom_table_header::sectionMoved,   this, &custom_table_header::handle_section_moved);
}

custom_table_header::~custom_table_header()
{

}

void custom_table_header::showEvent(QShowEvent *e)
{ // visibile thw widget
    for (int ii = 0; ii < count(); ii++)
    {
        if (!m_items[ii].item)
        {
            m_items[ii].item = new custom_table_header_widget(this);
        }
        else
        {
            m_items[ii].item->setParent(this);
        }

        m_items[ii].item->setGeometry(sectionViewportPosition(ii) + m_items[ii].margins.left,
                                      m_items[ii].margins.top,
                                      sectionSize(ii) - m_items[ii].margins.left - m_items[ii].margins.right - 1,
                                      height() - m_items[ii].margins.top - m_items[ii].margins.buttom - 1);
        m_items[ii].item->show();
    }

    QHeaderView::showEvent(e);
}

void custom_table_header::handle_section_resized(int i)
{ // resize and handling the header section
    int logical;

    for (int jj = visualIndex(i); jj < count(); jj++)
    {
        logical = logicalIndex(jj);

        m_items[logical].item->setGeometry(sectionViewportPosition(logical) + m_items[i].margins.left,
                                           m_items[i].margins.top,
                                           sectionSize(logical) - m_items[i].margins.left - m_items[i].margins.right - 1,
                                           height() - m_items[i].margins.top - m_items[i].margins.buttom - 1);
    }
}

void custom_table_header::handle_section_moved(int logical, int oldVisualIndex, int newVisualIndex)
{ // handling the section moved according to user
    Q_UNUSED(logical);

    for (int ii = qMin(oldVisualIndex, newVisualIndex); ii < count(); ii++)
    {
        int logical = logicalIndex(ii);
        m_items[logical].item->setGeometry(sectionViewportPosition(logical) + m_items[ii].margins.left,
                                           m_items[ii].margins.top,
                                           sectionSize(logical) - m_items[ii].margins.left - m_items[ii].margins.right - 1,
                                           height() - m_items[ii].margins.top - m_items[ii].margins.buttom - 1);
    }
}

void custom_table_header::set_scroll_positions()
{ // set the scroll position
    for(int ii = 0; ii < count(); ii++)
    {
        m_items[ii].item->setGeometry(sectionViewportPosition(ii) + m_items[ii].margins.left,
                                      m_items[ii].margins.top,
                                      sectionSize(ii) - m_items[ii].margins.left - m_items[ii].margins.right - 1,
                                      height() - m_items[ii].margins.top - m_items[ii].margins.buttom - 1);
    }
}
// set the global ui of header
void custom_table_header::set_item_widget(int index, QWidget * widget)
{
    m_items[index].item = widget;
}

void custom_table_header::set_item_margins(int index, custom_table_header::Margins margins)
{
    m_items[index].margins = margins;
}

custom_table_header::Margins::Margins(int left, int right, int top, int buttom) :
    left(left),
    right(right),
    top(top),
    buttom(buttom)
{

}

custom_table_header::Item::Item() :
    item(nullptr)
{

}


