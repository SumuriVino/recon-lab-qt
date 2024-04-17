#include "m_treewidgetitem.h"

m_treewidgetitem::m_treewidgetitem(QWidget *parent)
{
    m_state = 0;
}

m_treewidgetitem::~m_treewidgetitem()
{

}

int m_treewidgetitem::previous_state()
{
    return m_state;
}

void m_treewidgetitem::set_state(int st)
{
    m_state = st;
}

