#ifndef M_TREEWIDGET_H
#define M_TREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QtCore>
#include <QTreeWidget>

class m_treewidget : public QTreeWidget
{
public:
    m_treewidget(QWidget *parent = 0);
    ~m_treewidget();
};

#endif // M_TREEWIDGET_H
