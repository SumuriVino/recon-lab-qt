
#ifndef QHEXEDIT_P_H
#define QHEXEDIT_P_H

/** \cond docNever */

#include <QtGui>
#include <QPoint>
#include <QWidget>
#include <QScrollArea>
#include <QMenu>
#include <QAction>
#include <QImage>
#include <QColorDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtGui>
#include <QClipboard>
#include <QApplication>
#include "header_gui_settings.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_structures.h"
#include "header_identical_2.h"
#include "header_global_variables_1.h"
#include "message_box/message_dialog.h"
#include "add_tags/add_tags.h"
#include "header_global_variables_2.h"

#include "recon_generic_functions/recon_helper_standard.h"      


class QHexEditPrivate : public QWidget
{
    Q_OBJECT

public:
    QHexEditPrivate(QScrollArea *parent);
    ~QHexEditPrivate();

    void setAddressOffset(uint64_t offset);
    int addressOffset();

    void setData(QByteArray const &data);
    QByteArray data();

    void setAddressAreaColor(QColor const &color);
    QColor addressAreaColor();

    void setHighlightingColor(QColor const &color);
    QColor highlightingColor();

    void setOverwriteMode(bool overwriteMode);
    bool overwriteMode();
    void setReadOnly(bool read_only);
    bool readOnly();

    void insert(int i, const QByteArray & ba);
    void insert(int i, char ch);
    void remove(int index, int len=1);

    void setAddressArea(bool addressArea);
    void setAddressWidth(int addressWidth);
    void setAsciiArea(bool asciiArea);
    void setHighlighting(bool mode);
    virtual void setFont(const QFont &font);

    // HERS-- SERACH---
    void search_hex(QByteArray data, uint64_t page_size, bool is_previous_clicked, bool is_fresh_search);

    // ASCII search
    void search_ascii(QString text, uint64_t page_size, bool is_previous_clicked, bool is_fresh_search);

    // GO TO OFFSET
    void go_to_offset(uint64_t position, uint64_t page_size);

    //Find Next
    void search_next(int index, int searched_data_size);

    // Find Previous
    void search_previous(int index, int searched_data_size);

    // Set Searched data to zero and remove highlighted data
    void set_searched_data_for_page(int page_size);

    // Tag List
    void pub_set_start_offset_clicked(bool is_clicked);

    void clear_block_index_list();
    void set_current_page_details(int page_no, uint64_t page_start_offset, uint64_t page_end_offset);
    void set_the_list_of_already_set_blocks(QList<struct_global_hex_viewer_start_and_end_index> list);
    void set_context_menu(bool val);
    void add_selected_bytes_to_block(QString block_name, QString color_name);
    void set_block_db(QString db_path);
    void pub_set_essentials();

    void pub_set_start_offset(uint64_t offset);
    void highlight_searched_hex_data(QByteArray data, uint64_t page_size, int cursor_pos_index, bool is_fresh_search);
    void highlight_searched_ascii_text(QString text, uint64_t page_size, int cursor_pos_index);

    void pub_set_informative_dialog_status(bool status);
signals:
    void currentAddressChanged(int address);
    void currentSizeChanged(int size);
    void dataChanged();
    void overwriteModeChanged(bool state);
    void current_position_hex(QByteArray hex_val);
    void index_list_size(int);
    void current_offset(int);
    void selected_bytes_offset(int, int, bool);
    void signal_tag_start_offset_clicked(int);
    void export_selected_bytes_offset(QByteArray);
    void signal_add_tags_to_selected_bytes(QString tag_name_val, QString color_name);
    void signal_insert_tag_selected_bytes_info_in_db(int start_index, int end_index, QString tag_name, QString color_name);
    void signal_select_tag_name_for_tag_end_offset(QString tag_name_val, QString color_name);
    void signal_tag_end_offset_clicked(QString tag_name, QString color_name, uint64_t start_index, uint64_t end_index);
    void signal_update_column_index_in_upper_label(int char_width, int char_heigth, int no_of_hex_bytes_per_line, int start_of_coumn_index_text_offset, bool is_display_in_hex);

    void signal_page_end_reached(bool status);
    void signal_cursor_current_pos(qint64 pos);


protected:
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent *p_event);
    void mouseMoveEvent(QMouseEvent *p_event);
    void paintEvent(QPaintEvent *event);
    void setCursorPos(int position);
    void contextMenuEvent(QContextMenuEvent *p_event);

    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void updateCursor();
    void slot_copy_selected_bytes();
    void slot_copy_selected_text_as_ascii();
    void slot_select_all();

    void slot_tag_start_offset_triggered();
    void slot_tag_end_offset_submenu_triggered(QAction *clicked_tag_name);
    void slot_export_selected_bytes_triggered();
    void slot_tag_selected_bytes_submenu_triggered(QAction *action_name);
    void slot_create_new_tag_for_selected_bytes();
    void slot_add_tag_to_selected_bytes(QString tag_name, QString color_name);
    void slot_update_tag_selected_bytes_submenu(bool status, QString new_tag, QString tag_colour);

    void slot_select_tag_name_for_tag_end_offset(QString tag_name, QString color_name);
    void slot_create_new_tag_for_end_offset();
    void slot_update_tag_end_offset_submenu(bool status, QString new_tag, QString tag_colour);
    void slot_change_number_bytes_display_submenu_triggered(QAction *action_name);
    void slot_action_action_display_address_in_decimal_triggered();
    void slot_action_action_display_address_in_hex_triggered();
    void slot_abouttoshow_tags_in_submenu();
private:
    void adjust();
    int Point2Char(QPoint pos);

    QColor _addressAreaColor;
    QByteArray _data;
    QByteArray _originalData;
    QColor _highlightingColor;
    QScrollArea *_scrollArea;
    QTimer _cursorTimer;
    QPoint sel_origin;
    QPoint sel_start;
    QPoint sel_end;
    QMenu *p_menu_copy;
    QAction *p_action_copy_selected_bytes;
    QAction *p_action_copy_selected_text_ascii;

    QAction *p_action_select_all;

    // Add Tag
    QAction *p_action_tag_start_offset;
    QMenu *p_menu_tag_end_offset;
    QAction *p_action_export_selected_bytes;

    QMenu *p_menu_tag_selected_bytes;
    QMenu *p_menu_change_number_bytes_display;
    QMenu *p_menu_display_address_in_hex_or_decimal;
    QAction *p_action_display_address_in_hex;
    QAction *p_action_display_address_in_decimal;

    QPoint search_start;
    QPoint search_end;

    bool _blink;
    bool _addressArea;
    bool _asciiArea;
    bool _highlighting;
    bool _overwriteMode;
    bool _readOnly;

    int _addressNumbers, _realAddressNumbers;
    uint64_t _addressOffset;
    int _charWidth, _charHeight;
    int _cursorX, _cursorY, _cursorPosition;
    int _xPosAdr, _xPosHex, _xPosAscii;
    int _size;

    // SELECTION
    void set_cursor_pos_on_searched_text(uint64_t position, const uint64_t page_size);

    int Point2Char_for_search(QPoint pos);    

    QStringList index_list;
    int current_cursor_index = 0;
    int searched_data_size = 0;

    uint64_t data_size = 0;

    struct_global_hex_viewer_start_and_end_index struct_obj;
    QList<struct_global_hex_viewer_start_and_end_index> list_struct_indexs;

    int current_page_no;
    uint64_t current_page_start_offset;
    uint64_t current_page_end_offset;
    void highlight_block_bytes_fun(int cur_char, QPainter &painter);

    bool is_context_menu_full;
    QString block_db_path;
    uint64_t get_start_offset_of_block(QString block_name);

    message_dialog *message_dialog_obj;
    void display_tag_selected_bytes_submenu();

    add_tags *add_tags_for_selected_bytes_obj;

    uint64_t tag_start_offset;
    uint64_t tag_end_offset;

    bool start_offset_clicked;

    add_tags *add_tags_for_end_offset_obj;
    void display_tag_end_offset_submenu();
    void add_actions_in_display_bytes_menu();

    int HEXCHARS_IN_LINE_int;
    int GAP_HEX_ASCII_int;
    int BYTES_PER_LINE_int;
    int GAP_ADR_HEX_int;
    bool bool_is_address_display_in_hex;

    bool bool_show_informative_dialog;
    recon_helper_standard *recon_helper_standard_obj;

};

/** \endcond docNever */

#endif

