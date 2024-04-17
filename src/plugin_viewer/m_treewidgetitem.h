#ifndef MYTREEWIDGETITEM_H
#define MYTREEWIDGETITEM_H

#include <QTreeWidgetItem>
class m_treewidgetitem : public QTreeWidgetItem
{
public:
    m_treewidgetitem(QWidget *parent = 0);
    ~m_treewidgetitem();

    int previous_state();

    void set_state(int);

    int m_state;
};

#endif // MYTREEWIDGETITEM_H
