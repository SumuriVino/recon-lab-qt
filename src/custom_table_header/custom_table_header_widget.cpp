#include "custom_table_header_widget.h"

custom_table_header_widget::custom_table_header_widget(QWidget *parent) : QWidget(parent)
{
    column_sort_order = 1; ///----- 1 meaning asscending order-----///
    hbox_upper = new QHBoxLayout;
    hbox_upper->setSpacing(2);

    pushbutton_sort = new QPushButton(this);
    pushbutton_sort->setMaximumWidth(15);

    pushbutton_sort->setIcon(QIcon("../icons/signs/column_sort.png"));
    pushbutton_sort->setStyleSheet("background-color: rgba(255,255,255,0);");

    label_header_name = new QLabel();
    label_header_name->setFixedHeight(18);
    label_header_name->setAlignment(Qt::AlignCenter);

    hbox_upper->addWidget(pushbutton_sort);
    hbox_upper->addSpacing(5);
    hbox_upper->addWidget(label_header_name);

    line_edit_search_field = new QLineEdit;
    line_edit_search_field->setFixedHeight(20);
    line_edit_search_field->setClearButtonEnabled(true);
    line_edit_search_field->setPlaceholderText("Search");
    line_edit_search_field->setAttribute(Qt::WA_MacShowFocusRect, false);

    empty_label = new QLabel;

    vbox = new QVBoxLayout;
    vbox->addItem(hbox_upper);

    vbox->addWidget(line_edit_search_field);

    vbox->setContentsMargins(0,0,0,0);
    vbox->setAlignment(Qt::AlignBottom);
    vbox->setSpacing(0);

    setLayout(vbox);

    setMaximumHeight(50);

    connect(line_edit_search_field,SIGNAL(returnPressed()),this,SLOT(slot_line_edit_search_field_return_pressed()));
    connect(line_edit_search_field,SIGNAL(textChanged(QString)),this,SLOT(slot_line_edit_search_field_text_changed(QString)));
    connect(pushbutton_sort, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_sort_clicked(bool)));
}

void custom_table_header_widget::slot_pushbutton_sort_clicked(bool)
{
    if(column_sort_order == 1)
    {
        emit signal_sorting_clicked(column_sequence, column_sort_order);
        column_sort_order = 0;
        return;
    }
    else if(column_sort_order == 0)
    {
        emit signal_sorting_clicked(column_sequence, column_sort_order);
        column_sort_order = 1;
        return;
    }
}

void custom_table_header_widget::pub_set_column_name(QString col_name)
{
    empty_label->setFixedHeight(20); 

    if(col_name.isEmpty())
    {
        line_edit_search_field->hide();
        pushbutton_sort->hide();
        vbox->addWidget(empty_label);
        return;
    }

    label_header_name->setText(col_name);
}

void custom_table_header_widget::pub_set_column_name_for_file_system(QString col_name)
{
    empty_label->setFixedHeight(20);

    if(col_name.isEmpty() || col_name.contains("seen", Qt::CaseInsensitive))
    {
        line_edit_search_field->hide();
        pushbutton_sort->hide();
        vbox->addWidget(empty_label);
        return;
    }

    label_header_name->setText(col_name);
}

void custom_table_header_widget::pub_set_column_seq_number(int seq)
{
    column_sequence = seq;
}

void custom_table_header_widget::pub_set_icon_on_bookmark_or_seen(QPixmap icon)
{
    int width = label_header_name->width();

    label_header_name->setPixmap(icon.scaled (width,15,Qt::KeepAspectRatio));
    label_header_name->setAlignment(Qt::AlignBottom);
}

void custom_table_header_widget::slot_line_edit_search_field_return_pressed()
{
    emit search_field_return_pressed();
}

void custom_table_header_widget::slot_line_edit_search_field_text_changed(QString current_text)
{
    if(current_text.trimmed().size() < 1)
    {
        emit search_field_text_changed();
    }
}

void custom_table_header_widget::slot_line_edit_editing_finished()
{
    emit search_field_text_changed();
}

void custom_table_header_widget::pub_hide_search_field()
{
    line_edit_search_field->hide();
}

void custom_table_header_widget::pub_hide_empty_label()
{
    empty_label->hide();
}

void custom_table_header_widget::pub_clear_line_edit_search_field()
{
    line_edit_search_field->clear();
}

void custom_table_header_widget::pub_hide_pushbutton_sort()
{
    pushbutton_sort->hide();
}

QString custom_table_header_widget::pub_get_column_name()
{
    return this->label_header_name->text();
}

QString custom_table_header_widget::pub_get_search_text()
{
    return line_edit_search_field->text();
}
