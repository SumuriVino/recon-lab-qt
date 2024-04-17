#ifndef SQLITE_VIEWER_MANAGE_COLUMS_TYPE_H
#define SQLITE_VIEWER_MANAGE_COLUMS_TYPE_H

#include <QFrame>
#include "ui_custom_classes/m_tablewidget.h"
#include <QtCore>

namespace Ui {
class sqlite_viewer_manage_colums_type;
}

class sqlite_viewer_manage_colums_type : public QFrame
{
    Q_OBJECT

public:
    explicit sqlite_viewer_manage_colums_type(QWidget *parent = 0);
    ~sqlite_viewer_manage_colums_type();

    void pub_set_widget_and_tab_name(QTableWidget *tableWidget_ptr, QString tab_name);

private:
    Ui::sqlite_viewer_manage_colums_type *ui;
};

#endif // sqlite_viewer_manage_colums_type
