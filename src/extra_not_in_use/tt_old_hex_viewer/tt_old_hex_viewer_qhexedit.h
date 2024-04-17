
#ifndef QHEXEDIT_H
#define QHEXEDIT_H

#include <QtGui>
#include <QHBoxLayout>

#include "tt_old_hex_viewer_qhexedit_p.h"
                                   


class QHexEdit : public QScrollArea
{
    Q_OBJECT
    /*! Property data holds the content of QHexEdit. Call setData() to set the
    content of QHexEdit, data() returns the actual content.
    */
    Q_PROPERTY(QByteArray data READ data WRITE setData)

    /*! Property addressOffset is added to the Numbers of the Address Area.
    A offset in the address area (left side) is sometimes usefull, whe you show
    only a segment of a complete memory picture. With setAddressOffset() you set
    this property - with addressOffset() you get the actual value.
    */
    Q_PROPERTY(int addressOffset READ addressOffset WRITE setAddressOffset)

    /*! Property address area color sets (setAddressAreaColor()) the backgorund
    color of address areas. You can also read the color (addressaAreaColor()).
    */
    Q_PROPERTY(QColor addressAreaColor READ addressAreaColor WRITE setAddressAreaColor)

    /*! Property highlighting color sets (setHighlightingColor()) the backgorund
    color of highlighted text areas. You can also read the color
    (highlightingColor()).
    */
    Q_PROPERTY(QColor highlightingColor READ highlightingColor WRITE setHighlightingColor)

    /*! Porperty overwrite mode sets (setOverwriteMode()) or gets (overwriteMode()) the mode
    in which the editor works. In overwritem mode the user will overwrite existing data.
    */
    Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode)

    /*! Porperty read only sets (setReadOnly()) or gets (readOnly()) the
    current editable mode.
    */
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)

public:
    /*! Creates an instance of QHexEdit.
    \param parent Parent widget of QHexEdit.
    */
    QHexEdit(QWidget *parent = 0);

    /*! Inserts a byte array.
    \param i Index position, where to insert
    \param ba byte array, which is to insert
    */
    void insert(int i, const QByteArray & ba);

    /*! Inserts a char.
    \param i Index position, where to insert
    \param ch Char, which is to insert
    */
    void insert(int i, char ch);

    /*! Removes len bytes from the content.
    \param pos Index position, where to remove
    \param len Amount of bytes to remove
    */
    void remove(int pos, int len=1);

    /*! Set the font of the widget. Please use fixed width fonts like Mono or Courier.*/
    void setFont(const QFont &);

    /*! \cond docNever */
    void setAddressOffset(uint64_t offset);
    int addressOffset();
    void setData(QByteArray const &data);
    QByteArray data();
    void setAddressAreaColor(QColor const &color);
    QColor addressAreaColor();
    void setHighlightingColor(QColor const &color);
    QColor highlightingColor();
    void setOverwriteMode(bool);
    bool overwriteMode();
    void setReadOnly(bool);
    bool readOnly();
    /*! \endcond docNever */

    void set_block_db_path(QString block_db_path);
    void pub_set_essentials();

signals:
    void hex_value_of_cur(QByteArray);
    void size_index_list(int);
    void current_cur_offset(int);
    void bytes_offset_selected(int, int, bool);

    /*! Contains the address, where the cursor is located. */
    void currentAddressChanged(int address);

    /*! Contains the size of the data to edit. */
    void currentSizeChanged(int size);

    /*! The signal is emited every time, the data is changed. */
    void dataChanged();

    /*! The signal is emited every time, the overwrite mode is changed. */
    void overwriteModeChanged(bool state);

    void signal_hex_edit_tag_start_offset(int);
    void signal_export_selected_bytes(QByteArray);
    void signal_insert_selected_bytes_info_in_db(int start_index, int end_index, QString tag_name, QString color_name);
    void signal_hex_edit_tag_end_offset_clicked(QString tag_name, QString color_name, uint64_t start_index, uint64_t end_index);
    void signal_page_end_reached_for_search_highlight(bool status);
    void signal_cursor_current_pos_for_previous_click(qint64 pos);



public slots:

    /*! Set the minimum width of the address area.
      \param addressWidth Width in characters.
      */
    void setAddressWidth(int addressWidth);

    /*! Switch the address area on or off.
      \param addressArea true (show it), false (hide it).
      */
    void setAddressArea(bool addressArea);

    /*! Switch the ascii area on or off.
      \param asciiArea true (show it), false (hide it).
      */
    void setAsciiArea(bool asciiArea);

    /*! Switch the highlighting feature on or of.
      \param mode true (show it), false (hide it).
      */
    void setHighlighting(bool mode);

    // ------ For searching hex values---
    void slot_search_hex(QByteArray bytes, uint64_t page_size, bool previous_clicked, bool is_fresh_search);


    // For searching ASCII values
    void slot_search_ascii(QString text, uint64_t page_size, bool previous_clicked, bool is_fresh_search);



    // FOR DISPLAYING VARIOUS VALUES
    void slot_mouse_hex(QByteArray hex_val);

    // FOr go to offset
    void slot_go_to_offset(uint64_t offset, uint64_t page_size);

    // Find Next
    void slot_search_next(int search_index, int searched_data_size);

    //Find Previous
    void slot_search_previous(int search_index, int searched_data_size);

    // INDEX LIST SIZE--
    void slot_hex_list_size(int index_list_size);

    // Current cursor offset
    void slot_current_offset(int offset);

    // Clear Seletion
    void slot_set_page_search_clear(int page_size);

    // Selected Bytes offset
    void slot_selected_bytes_offset(int selection_start, int selection_end, bool selection);

    void slot_hex_edit_tag_start_offset(int current_offset);

    // Set Tag List
    void slot_tag_start_index_clicked(bool start_clicked);

    // Block End
    void slot_change_offset_value(uint64_t offset);

    void slot_clear_block_list();
    void slot_current_page_details(int page_no, uint64_t start_offset, uint64_t end_offset);
    void slot_export_selected_bytes(QByteArray selected_bytes);
    void slot_set_already_set_blocks(QList<struct_global_hex_viewer_start_and_end_index> list);
    void slot_set_context_menu(bool set_val);
    void slot_add_selected_bytes_to_block_clicked(QString block_name, QString color_name);
    void slot_insert_selected_bytes_info_in_db(int start_index, int end_index, QString tag_name, QString color_name);

    void slot_hex_edit_tag_end_offset_clicked(QString tag_name, QString color_name, uint64_t start_index, uint64_t end_index);

    void slot_hex_edit_set_tag_start_offset(uint64_t offset);
    void slot_qhexedit_update_ui_label_for_column_index(int char_width, int char_heigth, int no_of_hex_bytes_per_line, int start_of_coumn_index_text_offset, bool is_display_in_hex);

private slots:
    void slot_highlight_ascii_text_on_search(QString text, uint64_t page_size);
    void slot_highlight_hex_data_on_search(QByteArray data, uint64_t page_size);

    void slot_set_informative_dialog_status(bool status);
private:
    /*! \cond docNever */
    QHexEditPrivate *qHexEdit_p;
    QHBoxLayout *layout;
    QScrollArea *scrollArea;
    /*! \endcond docNever */
    void set_ui_for_display();
    QLabel *label_hex_bytes_column_start_pedding,*label_hex_bytes_column_end_pedding,*label_hex_bytes_column_index;

};

#endif

