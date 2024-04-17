#ifndef QHEXEDIT_TEXTVIEWER_H
#define QHEXEDIT_TEXTVIEWER_H

#include <QAbstractScrollArea>
#include <QPen>
#include <QBrush>

#include "chunks.h"
#include <QMenu>
#include "header_gui_settings.h"
#include "header_identical_2.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "add_tags/add_tags.h"

#include "recon_generic_functions/recon_helper_standard.h"
#include "file_viewer/hex_viewer/hex_viewer_complete/Scrollbar64.h"

class QHexEdit_textviewer : public QAbstractScrollArea
{
    Q_OBJECT

    /*! Property address area switch the address area on or off. Set addressArea true
    (show it), false (hide it).
    */
    Q_PROPERTY(bool addressArea READ addressArea WRITE setAddressArea_textview)

    /*! Property address area color sets (setAddressAreaColor()) the backgorund
    color of address areas. You can also read the color (addressaAreaColor()).
    */
    Q_PROPERTY(QColor addressAreaColor_textviewer READ addressAreaColor_textviewer WRITE setAddressAreaColor_textviewer)

    /*! Property addressOffset is added to the Numbers of the Address Area.
    A offset in the address area (left side) is sometimes usefull, whe you show
    only a segment of a complete memory picture. With setAddressOffset() you set
    this property - with addressOffset() you get the current value.
    */
    Q_PROPERTY(qint64 addressOffset_textviewer READ addressOffset_textviewer WRITE setAddressOffset_textviewer)

    /*! Set and get the minimum width of the address area, width in characters.
    */
    Q_PROPERTY(int addressWidth READ addressWidth WRITE setAddressWidth_textviewer)

    /*! Switch the ascii area on (true, show it) or off (false, hide it).
    */
    Q_PROPERTY(bool asciiArea READ asciiArea WRITE setAsciiArea_textview)

    /*! Set and get bytes number per line.*/
    Q_PROPERTY(int bytesPerLine READ bytesPerLine WRITE setBytesPerLine)

    /*! Porperty cursorPosition sets or gets the position of the editor cursor
    in QHexEdit. Every byte in data has to cursor positions: the lower and upper
    Nibble. Maximum cursor position is factor two of data.size().
    */
    Q_PROPERTY(qint64 cursorPosition READ cursorPosition WRITE setCursorPosition)

    /*! Property data holds the content of QHexEdit. Call setData() to set the
    content of QHexEdit, data() returns the actual content. When calling setData()
    with a QByteArray as argument, QHexEdit creates a internal copy of the data
    If you want to edit big files please use setData(), based on QIODevice.
    */
    Q_PROPERTY(QByteArray data_textviewer READ data_textviewer WRITE setData_text_viewer NOTIFY dataChanged)

    /*! That property defines if the hex values looks as a-f if the value is false(default)
    or A-F if value is true.
    */
    Q_PROPERTY(bool hexCaps READ hexCaps WRITE setHexCaps)

    /*! Property defines the dynamic calculation of bytesPerLine parameter depends of width of widget. 
    set this property true to avoid horizontal scrollbars and show the maximal possible data. defalut value is false*/
    Q_PROPERTY(bool dynamicBytesPerLine READ dynamicBytesPerLine WRITE setDynamicBytesPerLine)

    /*! Switch the highlighting feature on or of: true (show it), false (hide it).
    */
    Q_PROPERTY(bool highlighting READ highlighting WRITE setHighlighting)

    /*! Property highlighting color sets (setHighlightingColor()) the backgorund
    color of highlighted text areas. You can also read the color
    (highlightingColor()).
    */
    Q_PROPERTY(QColor highlightingColor_textviewer READ highlightingColor_textviewer WRITE setHighlightingColor_textviewer)


    /*! Property selection color sets (setSelectionColor()) the backgorund
    color of selected text areas. You can also read the color
    (selectionColor()).
    */
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)

    /*! Porperty readOnly sets (setReadOnly()) or gets (isReadOnly) the mode
    in which the editor works. In readonly mode the the user can only navigate
    through the data and select data; modifying is not possible. This
    property's default is false.
    */
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly_textview)

    /*! Set the font of the widget. Please use fixed width fonts like Mono or Courier.*/
    Q_PROPERTY(QFont font READ font WRITE setFont_textviewer)

public:
    /*! Creates an instance of QHexEdit.
    \param parent Parent widget of QHexEdit.
    */
    QHexEdit_textviewer(QWidget *parent=0);

    // Access to data of qhexedit

    /*! Sets the data of QHexEdit. The QIODevice will be opend just before reading
    and closed immediately afterwards. This is to allow other programs to rewrite
    the file while editing it.
    */
    bool setData_text_viewer(QIODevice &iODevice);



    // Char handling

    /*! Inserts a char.
    \param pos Index position, where to insert
    \param ch Char, which is to insert
    The char will be inserted and size of data grows.
    */
    void insert(qint64 pos, char ch);

    /*! Removes len bytes from the content.
    \param pos Index position, where to remove
    \param len Amount of bytes to remove
    */
    void remove(qint64 pos, qint64 len=1);

    /*! Replaces a char.
    \param pos Index position, where to overwrite
    \param ch Char, which is to insert
    The char will be overwritten and size remains constant.
    */
    void replace(qint64 pos, char ch);


    // ByteArray handling

    /*! Inserts a byte array.
    \param pos Index position, where to insert
    \param ba QByteArray, which is to insert
    The QByteArray will be inserted and size of data grows.
    */
    void insert(qint64 pos, const QByteArray &ba);

    /*! Replaces \param len bytes with a byte array \param ba
    \param pos Index position, where to overwrite
    \param ba QByteArray, which is inserted
    \param len count of bytes to overwrite
    The data is overwritten and size of data may change.
    */
    void replace(qint64 pos, qint64 len, const QByteArray &ba);


    // Utility functioins
    /*! Calc cursor position from graphics position
     * \param point from where the cursor position should be calculated
     * \return Cursor postioin
     */
    qint64 cursorPosition(QPoint point);

    /*! Ensure the cursor to be visble
     */
    void ensureVisible();

    /*! Set Font of QHexEdit
     * \param font
     */
    void setFont_textviewer(const QFont &font);




public slots:
    /*! Redoes the last operation. If there is no operation to redo, i.e.
      there is no redo step in the undo/redo history, nothing happens.
      */
    void redo();

    /*! Undoes the last operation. If there is no operation to undo, i.e.
      there is no undo step in the undo/redo history, nothing happens.
      */
    void undo();

signals:

    /*! Contains the address, where the cursor is located. */
    void currentAddressChanged(qint64 address);

    /*! Contains the size of the data to edit. */
    void currentSizeChanged(qint64 size);

    /*! The signal is emitted every time, the data is changed. */
    void dataChanged();

    void signal_add_tag_to_tag_search(qint64 start_offset , qint64 end_offset,QString tag_name, QString tag_color, QString color_hex, QByteArray tagged_content);


/*! \cond docNever */
public:
    ~QHexEdit_textviewer();

    // Properties
    bool addressArea();
    void setAddressArea_textview(bool addressArea);

    QColor addressAreaColor_textviewer();
    void setAddressAreaColor_textviewer(const QColor &color);

    qint64 addressOffset_textviewer();
    void setAddressOffset_textviewer(qint64 addressArea);

    int addressWidth();
    void setAddressWidth_textviewer(int addressWidth);

    bool asciiArea();
    void setAsciiArea_textview(bool asciiArea);

    int bytesPerLine();
    void setBytesPerLine(int count);

    qint64 cursorPosition();
    void setCursorPosition(qint64 position);

    QByteArray data_textviewer();
    void setData_text_viewer(const QByteArray &ba);

    void setHexCaps(const bool isCaps);
    bool hexCaps();

    void setDynamicBytesPerLine(const bool isDynamic);
    bool dynamicBytesPerLine();

    bool highlighting();
    void setHighlighting(bool mode);

    QColor highlightingColor_textviewer();
    void setHighlightingColor_textviewer(const QColor &color);

    bool isReadOnly();
    void setReadOnly_textview(bool readOnly);

    QColor selectionColor();
    void setSelectionColor(const QColor &color);

    void clear_text_viewer();
    void pub_set_essentials();

protected:
    // Handle events
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);
    virtual bool focusNextPrevChild(bool next);
private:
    // Handle selections
    void resetSelection(qint64 pos);            // set selectionStart and selectionEnd to pos
    void resetSelection();                      // set selectionEnd to selectionStart
    void setSelection(qint64 pos);              // set min (if below init) or max (if greater init)
    int getSelectionBegin();
    int getSelectionEnd();

    // Private utility functions
    void init();
    void readBuffers();

private slots:
    void adjust_data();                              // recalc pixel positions
    void dataChangedPrivate(int idx=0);        // emit dataChanged() signal
    void refresh();                             // ensureVisible() and readBuffers()
    void updateCursor();                        // update blinking cursor

    void slot_right_click_main_menu_triggered(QAction *current_clicked_action);
    void slot_act_create_tags();
    void slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour);
    void slot_add_tags(QString tag_data, QString colour);
    void slot_sub_menu_tags_clicked(QAction *current_clicked_action);

private:

    struct struct_text_viewer_start_and_end_index
    {
        uint64_t start_index;
        uint64_t end_index;
        QColor block_highlighting_color;

    };

    struct_text_viewer_start_and_end_index struct_obj;
    QList<struct_text_viewer_start_and_end_index> list_struct_indexes;

    // Name convention: pixel positions start with _px
    int _pxCharWidth, _pxCharHeight;            // char dimensions (dpendend on font)
    int _pxPosHexX;                             // X-Pos of HeaxArea
    int _pxPosAdrX;                             // X-Pos of Address Area
    int _pxPosAsciiX;                           // X-Pos of Ascii Area
    int _pxGapAdr;                              // gap left from AddressArea
    int _pxGapAdrHex;                           // gap between AddressArea and HexAerea
    int _pxGapHexAscii;                         // gap between HexArea and AsciiArea
    int _pxCursorWidth;                         // cursor width
    int _pxSelectionSub;                        // offset selection rect
    int _pxCursorX;                             // current cursor pos
    int _pxCursorY;                             // current cursor pos

    // Name convention: absolute byte positions in chunks start with _b
    qint64 _bSelectionBegin;                    // first position of Selection
    qint64 _bSelectionEnd;                      // end of Selection
    qint64 _bSelectionInit;                     // memory position of Selection
    qint64 _bPosFirst;                          // position of first byte shown
    qint64 _bPosLast;                           // position of last byte shown
    qint64 _bPosCurrent;                        // current position

    // variables to store the property values
    bool _addressArea;                          // left area of QHexEdit
    QColor _addressAreaColor;
    int _addressWidth;
    bool _asciiArea;
    qint64 _addressOffset_textviewer;
    int _bytesPerLine;
    int _hexCharsInLine;
    bool _highlighting;
    QBrush _brushSelection;
    QPen _penSelection;
    QBrush _brushHighlighted;
    QPen _penHighlighted;
    bool _readOnly;
    bool _hexCaps;
    bool _dynamicBytesPerLine;

    // other variables
    bool _editAreaIsAscii;                      // flag about the ascii mode edited
    int _addrDigits;                            // real no of addressdigits, may be > addressWidth
    bool _blink;                                // help get cursor blinking
    QBuffer _bData_textviewer;                             // buffer, when setup with QByteArray
    Chunks *_chunks;                            // IODevice based access to data
    QTimer _cursorTimer;                        // for blinking cursor
    qint64 _cursorPosition;                     // absolute positioin of cursor, 1 Byte == 2 tics
    QRect _cursorRect;                          // physical dimensions of cursor
    QByteArray _dataShown;                      // data in the current View
    QByteArray _markedShown;                    // marked data in view
    int _rowsShown;                             // lines of text shown

    QMenu *mainMenu;
    QMenu *sub_menu_tags;

    ///-----Tags----
    QAction *act_create_tag;
    QAction *act_remove_tag;

    add_tags *add_tags_module_obj;

    qint64 start_index_tag ;
    qint64 end_index_tag ;
    QString color_hex_tag;

    void create_tags_submenu();
    void update_tags_value(QString tag_data, QString colour);
    void action_submenu_tag_triggered(QString tag_name);

    void set_tag_color_text_viewer(int cur_char, QPainter &painter);

    recon_helper_standard *recon_helper_standard_obj;

    Scrollbar64 *scroll_bar_64_obj = NULL;
};

#endif
