#include "m_barset.h"



m_barset::m_barset(const QString label, QObject *parent): QBarSet(label,parent)
{

}

m_barset::~m_barset()
{

}


void m_barset::set_tooltip(QMap<int, QString> map)
{
    m_map = map;
}

QMap<int, QString> m_barset::get_tooltip()
{
    return m_map;
}



//child::child(int a) : parent(a)
// {
// }
