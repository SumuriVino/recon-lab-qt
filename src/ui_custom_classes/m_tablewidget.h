#ifndef M_TABLEWIDGET_H
#define M_TABLEWIDGET_H

#include <QTableWidget>
#include <QtCore>
#include <QtGui>
#include <QAbstractScrollArea>
#include <QHeaderView>
#include "custom_table_header/custom_table_header.h"

class m_tablewidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit m_tablewidget(QWidget *parent = 0);

    QList<QTableWidgetItem *> takeRow(int row);
    void setRow(int row, const QList<QTableWidgetItem *> &rowItems);

    void pub_set_id(int id);
    int pub_get_id();

    void pub_set_tab_name(QString tb_nm);
    QString pub_get_tab_name(void);

    void scrollContentsBy(int dx, int dy);
    void pub_set_horizontal_header_item_widget(int column, QWidget *widget);
    void pub_set_horizontal_header_item_margins(int column, custom_table_header::Margins margins);

protected:
    void keyPressEvent(QKeyEvent *event);

    
signals:
    void signal_arrow_key_pressed();
    void signal_bookmarked_key_pressed();

    void signal_control_m_key_pressed();
    void signal_control_n_key_pressed();
    void signal_control_e_key_pressed();
    void signal_control_a_key_pressed();
    void signal_spacebar_quicklook_pressed();


    
public slots:
    

private:

    int id_num;
    QString tab_name;

    int get_previous_visible_row(int row_index);
    int get_next_visible_row(int row_index);

    custom_table_header *custom_horizontal_header;
};

#endif // M_TABLEWIDGET_H
