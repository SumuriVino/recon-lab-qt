
#include <QtGui>

#include "tt_old_hex_viewer_qhexedit.h"


QHexEdit::QHexEdit(QWidget *parent) : QScrollArea(parent)
{
    set_ui_for_display();

    setWidgetResizable(true);

    connect(qHexEdit_p, SIGNAL(currentAddressChanged(int)), this, SIGNAL(currentAddressChanged(int)));
    connect(qHexEdit_p, SIGNAL(currentSizeChanged(int)), this, SIGNAL(currentSizeChanged(int)));
    connect(qHexEdit_p, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(qHexEdit_p, SIGNAL(overwriteModeChanged(bool)), this, SIGNAL(overwriteModeChanged(bool)));
    connect(qHexEdit_p,SIGNAL(current_position_hex(QByteArray)),this,SLOT(slot_mouse_hex(QByteArray)));
    connect(qHexEdit_p,SIGNAL(index_list_size(int)),this,SLOT(slot_hex_list_size(int)));
    connect(qHexEdit_p,SIGNAL(current_offset(int)),this,SLOT(slot_current_offset(int)));
    connect(qHexEdit_p,SIGNAL(selected_bytes_offset(int,int, bool)),this,SLOT(slot_selected_bytes_offset(int,int,bool)));
    connect(qHexEdit_p, SIGNAL(signal_tag_start_offset_clicked(int)), this, SLOT(slot_hex_edit_tag_start_offset(int)));
    connect(qHexEdit_p,SIGNAL(export_selected_bytes_offset(QByteArray)),this,SLOT(slot_export_selected_bytes(QByteArray)));
    connect(qHexEdit_p,SIGNAL(signal_insert_tag_selected_bytes_info_in_db(int, int, QString, QString)),this, SLOT(slot_insert_selected_bytes_info_in_db(int, int, QString, QString)));
    connect(qHexEdit_p,SIGNAL(signal_tag_end_offset_clicked(QString, QString, uint64_t, uint64_t)),this, SLOT(slot_hex_edit_tag_end_offset_clicked(QString, QString, uint64_t, uint64_t)));
    connect(qHexEdit_p,SIGNAL(signal_update_column_index_in_upper_label(int,int,int,int,bool)),this,SLOT(slot_qhexedit_update_ui_label_for_column_index(int,int,int,int,bool)));
    connect(qHexEdit_p,SIGNAL(signal_page_end_reached(bool)),this,SIGNAL(signal_page_end_reached_for_search_highlight(bool)));

    connect(qHexEdit_p,SIGNAL(signal_cursor_current_pos(qint64)),this,SIGNAL(signal_cursor_current_pos_for_previous_click(qint64)));



}

void QHexEdit::set_ui_for_display()
{
    qHexEdit_p = new QHexEditPrivate(this);

    QVBoxLayout *vboxLayout_all = new QVBoxLayout(this);
    QHBoxLayout *offset_top_Hboxlayout = new QHBoxLayout;

    label_hex_bytes_column_start_pedding = new QLabel(this);
    label_hex_bytes_column_start_pedding->setFixedWidth(40);
    label_hex_bytes_column_start_pedding->setStyleSheet("QLabel{background-color: #CACACA;color:#505050;}");

    label_hex_bytes_column_end_pedding = new QLabel(this);
    label_hex_bytes_column_end_pedding->setFixedWidth(40);
    label_hex_bytes_column_end_pedding->setStyleSheet("QLabel{background-color: #CACACA;color:#505050;}");

    label_hex_bytes_column_index = new QLabel(this);
    label_hex_bytes_column_index->setStyleSheet("QLabel{background-color: #CACACA;color:#505050;}");

    QFont font;
    font.setPixelSize(12);
    font.setFamily("Courier");
    font.setWordSpacing(-0.6);

    label_hex_bytes_column_index->setFont(font);
    label_hex_bytes_column_start_pedding->setFont(font);
    label_hex_bytes_column_end_pedding->setFont(font);

    offset_top_Hboxlayout->addWidget(label_hex_bytes_column_start_pedding);
    offset_top_Hboxlayout->addWidget(label_hex_bytes_column_index);
    offset_top_Hboxlayout->addWidget(label_hex_bytes_column_end_pedding);
    offset_top_Hboxlayout->setStretchFactor(label_hex_bytes_column_end_pedding,1);
    offset_top_Hboxlayout->setSpacing(0);
    offset_top_Hboxlayout->setMargin(0);

    vboxLayout_all->addLayout(offset_top_Hboxlayout);
    vboxLayout_all->addWidget(qHexEdit_p);
    vboxLayout_all->setMargin(0);
    vboxLayout_all->setSpacing(0);

    vboxLayout_all->setStretchFactor(qHexEdit_p,1);

    QWidget *parent_widget = new QWidget(this);
    parent_widget->setLayout(vboxLayout_all);

    setWidget(parent_widget);
}

void QHexEdit::insert(int i, const QByteArray & ba)
{
    qHexEdit_p->insert(i, ba);
}

void QHexEdit::insert(int i, char ch)
{
    qHexEdit_p->insert(i, ch);
}

void QHexEdit::remove(int pos, int len)
{
    qHexEdit_p->remove(pos, len);
}

void QHexEdit::setAddressArea(bool addressArea)
{
    qHexEdit_p->setAddressArea(addressArea);
}

void QHexEdit::setAddressWidth(int addressWidth)
{
    qHexEdit_p->setAddressWidth(addressWidth);
}

void QHexEdit::setAsciiArea(bool asciiArea)
{
    qHexEdit_p->setAsciiArea(asciiArea);
}

void QHexEdit::setHighlighting(bool mode)
{
    qHexEdit_p->setHighlighting(mode);
}

void QHexEdit::setAddressOffset(uint64_t offset)
{
    qHexEdit_p->setAddressOffset(offset);
}

int QHexEdit::addressOffset()
{
    return addressOffset();
}

void QHexEdit::setData(const QByteArray &data)
{
    qHexEdit_p->setData(data);
}

void QHexEdit::set_block_db_path(QString block_db_path)
{
    qHexEdit_p->set_block_db(block_db_path);
}

QByteArray QHexEdit::data()
{
    return qHexEdit_p->data();
}

void QHexEdit::setAddressAreaColor(const QColor &color)
{
    qHexEdit_p->setAddressAreaColor(color);
}

QColor QHexEdit::addressAreaColor()
{
    return qHexEdit_p->addressAreaColor();
}

void QHexEdit::setHighlightingColor(const QColor &color)
{
    qHexEdit_p->setHighlightingColor(color);
}

QColor QHexEdit::highlightingColor()
{
    return qHexEdit_p->highlightingColor();
}

void QHexEdit::setOverwriteMode(bool overwriteMode)
{
    qHexEdit_p->setOverwriteMode(overwriteMode);
}

bool QHexEdit::overwriteMode()
{
    return qHexEdit_p->overwriteMode();
}

void QHexEdit::setReadOnly(bool read_only)
{
  qHexEdit_p->setReadOnly(read_only);
}

bool QHexEdit::readOnly() {
  return qHexEdit_p->readOnly();
}

void QHexEdit::setFont(const QFont &font)
{
    qHexEdit_p->setFont(font);
}

void QHexEdit::slot_search_hex(QByteArray bytes, uint64_t page_size,bool previous_clicked, bool is_fresh_search)
{
    qHexEdit_p->search_hex(bytes, page_size,previous_clicked,is_fresh_search);
}

void QHexEdit::slot_search_ascii(QString text, uint64_t page_size, bool previous_clicked, bool is_fresh_search)
{
    qHexEdit_p->search_ascii(text, page_size,previous_clicked,is_fresh_search);
}

void QHexEdit::slot_set_informative_dialog_status(bool status)
{
    qHexEdit_p->pub_set_informative_dialog_status(status);
}

void QHexEdit::slot_highlight_ascii_text_on_search(QString text, uint64_t page_size)
{
    qHexEdit_p->highlight_searched_ascii_text(text, page_size,0);

}

void QHexEdit::slot_highlight_hex_data_on_search(QByteArray data, uint64_t page_size)
{
    qHexEdit_p->highlight_searched_hex_data(data, page_size, 0, false);
}

void QHexEdit::slot_mouse_hex(QByteArray hex_val)
{
    emit hex_value_of_cur(hex_val);
}

void QHexEdit::slot_go_to_offset(uint64_t offset, uint64_t page_size)
{
    qHexEdit_p->go_to_offset(offset, page_size);
}

void QHexEdit::slot_search_next(int search_index, int searched_data_size)
{
    qHexEdit_p->search_next(search_index, searched_data_size);
}

void QHexEdit::slot_search_previous(int search_index, int searched_data_size)
{
    qHexEdit_p->search_previous(search_index, searched_data_size);
}

void QHexEdit::slot_hex_list_size(int index_list_size)
{
    emit size_index_list(index_list_size);
}

void QHexEdit::slot_current_offset(int offset)
{
    emit current_cur_offset(offset);
}

void QHexEdit::slot_hex_edit_tag_start_offset(int current_offset)
{
    emit signal_hex_edit_tag_start_offset(current_offset);
}

void QHexEdit::slot_set_page_search_clear(int page_size)
{
    qHexEdit_p->set_searched_data_for_page(page_size);
}

void QHexEdit::slot_tag_start_index_clicked(bool start_clicked)
{
    qHexEdit_p->pub_set_start_offset_clicked(start_clicked);
}

void QHexEdit::slot_hex_edit_set_tag_start_offset(uint64_t offset)
{
    qHexEdit_p->pub_set_start_offset(offset);
}

void QHexEdit::slot_selected_bytes_offset(int selection_start, int selection_end, bool selection)
{
    emit bytes_offset_selected(selection_start,selection_end, selection);
}

void QHexEdit::slot_change_offset_value(uint64_t offset)
{
    setAddressOffset(offset);
}

void QHexEdit::slot_clear_block_list()
{
    qHexEdit_p->clear_block_index_list();
}

void QHexEdit::slot_current_page_details(int page_no,uint64_t start_offset,uint64_t end_offset)
{
    qHexEdit_p->set_current_page_details(page_no,start_offset,end_offset);
}

void QHexEdit::slot_export_selected_bytes(QByteArray selected_bytes)
{
    emit signal_export_selected_bytes(selected_bytes);
}

void QHexEdit::slot_set_already_set_blocks(QList<struct_global_hex_viewer_start_and_end_index> list)
{
    qHexEdit_p->set_the_list_of_already_set_blocks(list);
}

void QHexEdit::slot_set_context_menu(bool set_val)
{
    qHexEdit_p->set_context_menu(set_val);
}

void QHexEdit::slot_insert_selected_bytes_info_in_db(int start_index,int end_index, QString tag_name, QString color_name)
{
    emit signal_insert_selected_bytes_info_in_db(start_index,end_index, tag_name, color_name);
}

void QHexEdit::slot_hex_edit_tag_end_offset_clicked(QString tag_name, QString color_name, uint64_t start_index, uint64_t end_index)
{
    emit signal_hex_edit_tag_end_offset_clicked(tag_name,color_name,start_index,end_index);
}

void QHexEdit::slot_qhexedit_update_ui_label_for_column_index(int char_width, int char_heigth, int no_of_hex_bytes_per_line, int start_of_coumn_index_text_offset, bool is_display_in_hex)
{    
    if(start_of_coumn_index_text_offset == 0)
        return;

    QString final_text;

    label_hex_bytes_column_start_pedding->setFixedWidth(start_of_coumn_index_text_offset + 2);

    if(is_display_in_hex)
    {
        for(int i = 0; i < no_of_hex_bytes_per_line; i++)
        {
            QString test = QString("%1").arg(i, 2, 16, QChar('0'));
            test = test.toUpper();

            final_text += test;
            final_text+= " ";
        }
    }
    else
    {
        for(int i = 0; i < no_of_hex_bytes_per_line; i++)
        {
            QString test = QString("%1").arg(i, 2, 10, QChar('0'));
            test = test.toUpper();

            final_text += test;
            final_text+= " ";
        }
    }

    label_hex_bytes_column_index->setText(final_text);
}

void QHexEdit::slot_add_selected_bytes_to_block_clicked(QString block_name,QString color_name)
{
    qHexEdit_p->add_selected_bytes_to_block(block_name,color_name);
}

void QHexEdit::pub_set_essentials()
{
    qHexEdit_p->pub_set_essentials();
}
