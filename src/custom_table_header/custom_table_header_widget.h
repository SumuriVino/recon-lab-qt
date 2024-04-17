#ifndef CUSTOM_TABLE_HEADER_WIDGET_H
#define CUSTOM_TABLE_HEADER_WIDGET_H

#include <QtCore>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

class custom_table_header_widget : public QWidget
{
    Q_OBJECT
public:
    explicit custom_table_header_widget(QWidget *parent = nullptr);

    void pub_set_column_name(QString col_name);
    void pub_set_column_name_for_file_system(QString col_name);
    void pub_set_icon_on_bookmark_or_seen(QPixmap icon);
    void pub_hide_search_field();
    void pub_hide_pushbutton_sort();
    void pub_clear_line_edit_search_field();
    void pub_set_column_seq_number(int);
    QString pub_get_column_name();
    QString pub_get_search_text();
    void pub_hide_empty_label();
    QPushButton *pushbutton_sort;

public slots:
    void slot_line_edit_search_field_return_pressed();
    void slot_line_edit_search_field_text_changed(QString current_text);
signals:
    void search_field_return_pressed();
    void search_field_text_changed();
    void signal_sorting_clicked(int,int);

protected:

private slots:
    void slot_line_edit_editing_finished();
    void slot_pushbutton_sort_clicked(bool);
private:
    QVBoxLayout *vbox;
    QLineEdit *line_edit_search_field;
    QLabel *label_header_name;
    int column_sequence;
    QHBoxLayout *hbox_upper;
    int column_sort_order;
    QLabel *empty_label;
};

#endif // CUSTOM_TABLE_HEADER_WIDGET_H
