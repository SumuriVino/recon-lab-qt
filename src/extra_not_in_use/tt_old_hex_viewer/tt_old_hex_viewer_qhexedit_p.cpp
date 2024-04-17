#include "tt_old_hex_viewer_qhexedit_p.h"

QHexEditPrivate::QHexEditPrivate(QScrollArea *parent) : QWidget(parent)
{
    int entered_bytes_size = 32;

    recon_helper_standard_obj = new recon_helper_standard(this);

    int no_of_hexchars_in_a_line = (2 * entered_bytes_size) + (entered_bytes_size - 1);

    HEXCHARS_IN_LINE_int = no_of_hexchars_in_a_line;
    GAP_HEX_ASCII_int = entered_bytes_size;
    BYTES_PER_LINE_int = entered_bytes_size;
    GAP_ADR_HEX_int = 10;
    bool_is_address_display_in_hex = true;

    bool_show_informative_dialog = true;

    _scrollArea = parent;
    setAddressWidth(4);
    setAddressOffset(0);
    setAddressArea(true);
    setAsciiArea(true);
    setHighlighting(true);
    setOverwriteMode(true);

    message_dialog_obj = new message_dialog(this);
    start_offset_clicked = false;

    add_tags_for_selected_bytes_obj = new add_tags(this);
    connect(add_tags_for_selected_bytes_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tag_selected_bytes_submenu(bool,QString,QString)));
    connect(add_tags_for_selected_bytes_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tag_to_selected_bytes(QString,QString)));
    connect(this,SIGNAL(signal_add_tags_to_selected_bytes(QString, QString)),this,SLOT(slot_add_tag_to_selected_bytes(QString, QString)));

    add_tags_for_end_offset_obj = new add_tags(this);
    connect(add_tags_for_end_offset_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tag_end_offset_submenu(bool,QString,QString)));
    connect(add_tags_for_end_offset_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_select_tag_name_for_tag_end_offset(QString,QString)));
    connect(this,SIGNAL(signal_select_tag_name_for_tag_end_offset(QString, QString)),this,SLOT(slot_select_tag_name_for_tag_end_offset(QString,QString)));

    setAddressAreaColor(QColor(Qt::lightGray).lighter(110));
    setHighlightingColor(QColor("#a1c9f7"));

    this->setReadOnly(true);
    this->sel_origin=QPoint(0,0);
    this->sel_start=QPoint(0,0);
    this->sel_end=QPoint(0,0);

    setFont(QFont("Courier", MACRO_QMenu_Font_Size_int));
    //    QFont m_font = this->font();
    //    m_font.setPointSize(11);
    //    setFont(m_font);

    connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursor()));

    _cursorTimer.setInterval(500);

    setFocusPolicy(Qt::StrongFocus);
    _size = -1;

    // Create context menu
    this->p_menu_copy                       = new QMenu(tr("Copy"),this);
    this->p_action_copy_selected_bytes      = new QAction(tr("Selected bytes"),this->p_menu_copy);
    this->p_action_copy_selected_text_ascii = new QAction(tr("Selected ASCII"),this->p_menu_copy);
    this->p_action_select_all               = new QAction(tr("Select all"),this);
    this->p_action_tag_start_offset         = new QAction(tr("Tag start offset"), this);
    this->p_menu_tag_end_offset             = new QMenu(tr("Tag end offset"),this);
    this->p_action_export_selected_bytes    = new QAction(tr("Export selected bytes"),this);
    this->p_menu_tag_selected_bytes         = new QMenu(tr("Tag selected bytes"),this);
    this->p_menu_change_number_bytes_display = new QMenu(tr("Display bytes"),this);
    this->p_menu_display_address_in_hex_or_decimal = new QMenu(tr("Display address area"),this);
    this->p_action_display_address_in_decimal = new QAction(tr("In decimal"),this->p_menu_display_address_in_hex_or_decimal);
    this->p_action_display_address_in_hex = new QAction(tr("In hex"),this->p_menu_display_address_in_hex_or_decimal);

    this->p_menu_copy->addAction(this->p_action_copy_selected_bytes);
    this->p_menu_copy->addAction(this->p_action_copy_selected_text_ascii);

    this->p_menu_display_address_in_hex_or_decimal->addAction(this->p_action_display_address_in_decimal);
    this->p_menu_display_address_in_hex_or_decimal->addAction(this->p_action_display_address_in_hex);

    this->p_menu_tag_end_offset->setEnabled(false);

    add_actions_in_display_bytes_menu();

    this->connect(this->p_action_copy_selected_bytes, SIGNAL(triggered()), this, SLOT(slot_copy_selected_bytes()));
    this->connect(this->p_action_copy_selected_text_ascii, SIGNAL(triggered()), this, SLOT(slot_copy_selected_text_as_ascii()));
    this->connect(this->p_action_select_all,SIGNAL(triggered()),this,SLOT(slot_select_all()));
    this->connect(this->p_action_tag_start_offset, SIGNAL(triggered()), this, SLOT(slot_tag_start_offset_triggered()));
    this->connect(this->p_menu_tag_end_offset, SIGNAL(triggered(QAction*)), this, SLOT(slot_tag_end_offset_submenu_triggered(QAction*)));
    this->connect(this->p_action_export_selected_bytes,SIGNAL(triggered()),this,SLOT(slot_export_selected_bytes_triggered()));
    this->connect(this->p_menu_tag_selected_bytes, SIGNAL(triggered(QAction*)), this, SLOT(slot_tag_selected_bytes_submenu_triggered(QAction*)));
    this->connect(this->p_menu_tag_selected_bytes, SIGNAL(aboutToShow()), this, SLOT(slot_abouttoshow_tags_in_submenu()));
    this->connect(this->p_menu_change_number_bytes_display, SIGNAL(triggered(QAction*)), this, SLOT(slot_change_number_bytes_display_submenu_triggered(QAction*)));
    this->connect(this->p_action_display_address_in_decimal, SIGNAL(triggered()), this, SLOT(slot_action_action_display_address_in_decimal_triggered()));
    this->connect(this->p_action_display_address_in_hex, SIGNAL(triggered()), this, SLOT(slot_action_action_display_address_in_hex_triggered()));

    this->setMouseTracking(true);
}

QHexEditPrivate::~QHexEditPrivate()
{
    // Delete context menu
    delete this->p_action_copy_selected_bytes;
    delete this->p_action_copy_selected_text_ascii;
    delete this->p_action_select_all;
    delete this->p_action_tag_start_offset;

    delete this->p_menu_copy;
    delete this->p_menu_tag_end_offset;
}

void QHexEditPrivate::setAddressOffset(uint64_t offset)
{
    _addressOffset = offset;
    adjust();
}

int QHexEditPrivate::addressOffset()
{
    return _addressOffset;
}

void QHexEditPrivate::pub_set_essentials()
{

    display_tag_selected_bytes_submenu();
    display_tag_end_offset_submenu();
}

void QHexEditPrivate::pub_set_informative_dialog_status(bool status)
{
    bool_show_informative_dialog = status;
}

void QHexEditPrivate::setData(const QByteArray &data)
{
    // Delete any previous selections

    index_list.clear();

    this->sel_origin.setX(0);
    this->sel_origin.setY(0);
    this->sel_start=this->sel_origin;
    this->sel_end=this->sel_origin;

    if(!data.isNull() && !data.isEmpty()) this->_cursorTimer.start();
    else this->_cursorTimer.stop();
    this->_data = data;
    this->_originalData = data;
    this->adjust();
    this->setCursorPos(0);

    //    this->setFocus();
}

QByteArray QHexEditPrivate::data()
{
    return _data;
}

void QHexEditPrivate::setAddressAreaColor(const QColor &color)
{
    _addressAreaColor = color;
    update();
}

QColor QHexEditPrivate::addressAreaColor()
{
    return _addressAreaColor;
}

void QHexEditPrivate::setHighlightingColor(const QColor &color)
{
    _highlightingColor = color;
    update();
}

QColor QHexEditPrivate::highlightingColor()
{
    return _highlightingColor;
}

void QHexEditPrivate::setOverwriteMode(bool overwriteMode)
{
    if (overwriteMode != _overwriteMode)
    {
        emit overwriteModeChanged(overwriteMode);
        _overwriteMode = overwriteMode;
        adjust();
    }
}

bool QHexEditPrivate::overwriteMode()
{
    return _overwriteMode;
}

void QHexEditPrivate::setReadOnly(bool read_only)
{
    this->_readOnly=read_only;
}

bool QHexEditPrivate::readOnly()
{
    return this->_readOnly;
}

void QHexEditPrivate::insert(int i, const QByteArray & ba)
{
    _data.insert(i, ba);
    _originalData.insert(i, ba);
}

void QHexEditPrivate::insert(int i, char ch)
{
    _data.insert(i, ch);
    _originalData.insert(i, ch);
}

void QHexEditPrivate::remove(int index, int len)
{
    _data.remove(index, len);
    _originalData.remove(index, len);
}

void QHexEditPrivate::setAddressArea(bool addressArea)
{
    _addressArea = addressArea;
    adjust();
    setCursorPos(_cursorPosition);
}

void QHexEditPrivate::setAddressWidth(int addressWidth)
{
    if ((addressWidth >= 0) and (addressWidth<=6))
    {
        _addressNumbers = addressWidth;
        adjust();
        setCursorPos(_cursorPosition);
    }
}

void QHexEditPrivate::setAsciiArea(bool asciiArea)
{
    _asciiArea = asciiArea;
    adjust();
}

void QHexEditPrivate::setFont(const QFont &font)
{
    QWidget::setFont(font);
    adjust();
}

void QHexEditPrivate::setHighlighting(bool mode)
{
    _highlighting = mode;
    update();
}

void QHexEditPrivate::keyPressEvent(QKeyEvent *event)
{
    bool down = false;
    int charX = (_cursorX - _xPosHex) / _charWidth;
    int posX = (charX / 3) * 2 + (charX % 3);
    int posBa = (_cursorY / _charHeight) * BYTES_PER_LINE_int + posX / 2;

    int key = int(event->text()[0].toLatin1());
    if (!this->_readOnly &&
            ((key>='0' && key<='9') || (key>='a' && key <= 'f')))
    {
        // insert char
        if (_overwriteMode == false)
            if ((charX % 3) == 0)
            {
                insert(posBa, char(0));
                adjust();
            }
        if (_data.size() > 0)
        {
            QByteArray hexValue = _data.mid(posBa, 1).toHex();
            if ((charX % 3) == 0)
                hexValue[0] = key;
            else
                hexValue[1] = key;
            _data.replace(posBa, 1, QByteArray().fromHex(hexValue));
            emit dataChanged();

            setCursorPos(_cursorPosition + 1);
            down = true;
        }
    }

    // delete char
    if (!this->_readOnly && event->matches(QKeySequence::Delete)) {
        remove(posBa);
    }
    if (!this->_readOnly && event->key() == Qt::Key_Backspace) {
        remove(posBa - 1);
        setCursorPos(_cursorPosition - 2);
    }

    // handle other function keys
    if(!this->_readOnly && event->key() == Qt::Key_Insert) {
        setOverwriteMode(!_overwriteMode);
        setCursorPos(_cursorPosition);
    }

    if (event->matches(QKeySequence::MoveToNextChar))
    {
        setCursorPos(_cursorPosition + 1);
        down = true;
    }
    if (event->matches(QKeySequence::MoveToPreviousChar))
    {
        setCursorPos(_cursorPosition - 1);
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToStartOfLine))
    {
        setCursorPos(_cursorPosition - (_cursorPosition % (2 * BYTES_PER_LINE_int)));
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToEndOfLine))
    {
        setCursorPos(_cursorPosition | (2 * BYTES_PER_LINE_int -1));
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToPreviousLine))
    {
        setCursorPos(_cursorPosition - (2 * BYTES_PER_LINE_int));
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToPreviousPage))
    {
        setCursorPos(_cursorPosition - (((_scrollArea->viewport()->height() / _charHeight) - 1) * 2 * BYTES_PER_LINE_int));
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToStartOfDocument))
    {
        setCursorPos(0);
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToNextLine))
    {
        setCursorPos(_cursorPosition + (2 * BYTES_PER_LINE_int));
        down = true;
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToEndOfDocument))
    {
        setCursorPos(_data.size() * 2);
        down = true;
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }
    if (event->matches(QKeySequence::MoveToNextPage))
    {
        setCursorPos(_cursorPosition + (((_scrollArea->viewport()->height() / _charHeight) - 1) * 2 * BYTES_PER_LINE_int));
        down = true;
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }

    if(event->matches(QKeySequence::MoveToNextChar))
    {
        setCursorPos(_cursorPosition);
        emit current_position_hex(_data.mid(_cursorPosition/2,8));
        emit current_offset(_cursorPosition/2);
    }

    // when we move downwards, we have to go a little further
    if (down)
    {
        _scrollArea->ensureVisible(_cursorX, _cursorY, 3, 3 + _charHeight);
    }
    else
    {
        _scrollArea->ensureVisible(_cursorX, _cursorY, 3, 3);
    }

    update();
}

void QHexEditPrivate::mouseReleaseEvent(QMouseEvent *event)
{
    emit current_position_hex(_data.mid(_cursorPosition/2,8));
    emit current_offset(_cursorPosition/2);
}

void QHexEditPrivate::mousePressEvent(QMouseEvent *p_event)
{
    if(p_event->button()==Qt::LeftButton)
    {
        // Init selection origin, start and end point
        this->sel_origin=p_event->pos();
        this->sel_end=this->sel_start=this->sel_origin;

        // Set cursor to current pos
        int curs_pos=this->Point2Char(this->sel_start);

        if(curs_pos!=-1)
            setCursorPos(curs_pos);

        this->search_start = this->search_end = this->sel_origin;
    }
    else
    {
        QWidget::mousePressEvent(p_event);
    }
}

void QHexEditPrivate::mouseMoveEvent(QMouseEvent *p_event)
{
    //    uint64_t current_pos = this->Point2Char(p_event->pos())/2;
    //    uint64_t current_page_start_with_pos = current_page_start_offset+current_pos;


    //    for(int i = 0; i < list_struct_indexs.size(); i++)
    //    {
    //        if(current_page_start_with_pos >= list_struct_indexs.at(i).block_actual_start_offset_in_file && current_page_start_with_pos <= list_struct_indexs.at(i).block_actual_end_offset_in_file)
    //        {
    //            this->setToolTip(list_struct_indexs.at(i).current_block_name);
    //        }
    //    }

    if(p_event->buttons()==Qt::LeftButton)
    {
        // Update current selection and repaint hexedit
        if(this->Point2Char(p_event->pos())>this->Point2Char(this->sel_origin))
        {
            this->sel_start=this->sel_origin;
            this->sel_end=p_event->pos();

            _scrollArea->ensureVisible(sel_end.x(), sel_end.y(), 3, 3 + _charHeight);
        }
        else
        {
            this->sel_end=this->sel_origin;
            this->sel_start=p_event->pos();

            _scrollArea->ensureVisible(sel_start.x(), sel_start.y(), 3, 3 + _charHeight);
        }

        this->update();
    }
    else
    {
        QWidget::mouseMoveEvent(p_event);
    }
}

void QHexEditPrivate::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    // Draw some patterns if needed
    painter.fillRect(event->rect(), this->palette().color(QPalette::Base));
    if(_addressArea)
    {
        painter.fillRect(QRect(_xPosAdr,
                               event->rect().top(),
                               _xPosHex - GAP_ADR_HEX_int+2,
                               height()),
                         _addressAreaColor);
    }


    if(_asciiArea)
    {
        int linePos=_xPosAscii - (GAP_HEX_ASCII_int / 2);
        painter.setPen(Qt::gray);
        painter.drawLine(linePos,event->rect().top(),linePos,height());
    }

    painter.setPen(this->palette().color(QPalette::WindowText));

    // Calc positions
    int firstLineIdx = ((event->rect().top() / _charHeight) - _charHeight) * BYTES_PER_LINE_int;

    if(firstLineIdx<0)
        firstLineIdx=0;

    int lastLineIdx = ((event->rect().bottom() / _charHeight) + _charHeight) *BYTES_PER_LINE_int;

    if(lastLineIdx>_data.size())
        lastLineIdx=_data.size();

    int yPosStart = ((firstLineIdx) / BYTES_PER_LINE_int) * _charHeight + _charHeight;

    // Paint address area
    if(_addressArea)
    {
        painter.setPen("#808080");
        for(int lineIdx = firstLineIdx, yPos = yPosStart ; lineIdx < lastLineIdx ; lineIdx += BYTES_PER_LINE_int, yPos += _charHeight)
        {
            if(bool_is_address_display_in_hex)
            {
                QString address=QString("%1").arg(lineIdx+_addressOffset,
                                                  _realAddressNumbers,
                                                  16,
                                                  QChar('0'));
                painter.drawText(_xPosAdr, yPos, address);
            }
            else
            {
                QString address=QString("%1").arg(lineIdx+_addressOffset,
                                                  _realAddressNumbers,
                                                  10,
                                                  QChar('0'));
                painter.drawText(_xPosAdr, yPos, address);
            }
        }
    }

    // Prepare values for a selection
    int selection_start=0,selection_end=0;
    bool selection=false;
    if(!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end)
    {
        selection_start=this->Point2Char(this->sel_start)/2;
        selection_end=this->Point2Char(this->sel_end)/2;

        if(selection_end >= _data.size())
            selection_end = _data.size() - 1;

        selection=true;
    }

    // Prepare values SEARCH selection
    int search_start=0,serch_end=0;
    bool selection_search=false;
    if(!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end)
    {
        search_start=this->Point2Char_for_search(this->search_start)/2;
        serch_end=this->Point2Char_for_search(this->search_end)/2;

        if(serch_end >= _data.size())
            serch_end = _data.size() - 1;

        selection_search=true;
    }

    if(selection)
        emit selected_bytes_offset(selection_start, selection_end,selection);
    else if(selection_search)
        emit selected_bytes_offset(search_start, serch_end,selection_search);
    else
        emit selected_bytes_offset(0, 0,false);

    emit current_offset(_cursorPosition/2);

    // Paint hex area
    QByteArray hexBa(_data.mid(firstLineIdx,lastLineIdx-firstLineIdx+1).toHex());
    QBrush highLighted=QBrush(_highlightingColor);

    for(int lineIdx = firstLineIdx, yPos = yPosStart; lineIdx < lastLineIdx; lineIdx += BYTES_PER_LINE_int, yPos += _charHeight)
    {
        QByteArray hex;
        int xPos=_xPosHex;
        for(int colIdx=0; ((lineIdx + colIdx) < _data.size() && (colIdx < BYTES_PER_LINE_int)); colIdx++)
        {
            highLighted=QBrush(_highlightingColor);
            painter.setBackground(highLighted);

            // Highlight diff bytes
            if(_highlighting)
            {
                int posBa=lineIdx + colIdx;
                if(posBa>=_originalData.size())
                {
                    painter.setBackgroundMode(Qt::TransparentMode);
                }
                else
                {
                    if(_data[posBa]==_originalData[posBa])
                    {
                        painter.setBackgroundMode(Qt::TransparentMode);
                    }
                    else
                    {
                        painter.setBackgroundMode(Qt::OpaqueMode);
                    }
                }
            }


            // Highlight selection
            if(selection)
            {
                int cur_char = lineIdx+colIdx;
                if(cur_char >= selection_start && cur_char <= selection_end)
                {
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }
                else
                {
                    painter.setBackgroundMode(Qt::TransparentMode);
                }
            }

            // Highlight SEARCH selection
            if(selection_search)
            {
                int cur_char = lineIdx+colIdx;
                if(cur_char >= search_start && cur_char <= serch_end)
                {
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }
                else
                {
                    painter.setBackgroundMode(Qt::TransparentMode);
                }
            }

            if(list_struct_indexs.size() > 0)
            {
                int cur_char = lineIdx+colIdx;
                highlight_block_bytes_fun(cur_char,painter);
            }

            // Change color of alternate column hex
            int line_index = colIdx%2;
            if(line_index == 0)
                painter.setPen(Qt::black);
            else
                painter.setPen(Qt::darkYellow);

            // Render hex value
            if(colIdx==0)
            {
                hex=hexBa.mid((lineIdx-firstLineIdx)*2,2);
                hex = hex.toUpper();
                painter.drawText(xPos,yPos,hex);
                xPos+=2*_charWidth;
            }
            else
            {
                hex=hexBa.mid((lineIdx+colIdx-firstLineIdx)*2,2).prepend(" ");
                hex = hex.toUpper();
                painter.drawText(xPos,yPos,hex);
                xPos+=3*_charWidth;
            }
        }
    }

    // Paint ascii area
    if(_asciiArea)
    {
        for(int lineIdx=firstLineIdx, yPos=yPosStart; lineIdx<lastLineIdx; lineIdx+=BYTES_PER_LINE_int, yPos+=_charHeight)
        {
            QByteArray ascii=_data.mid(lineIdx,BYTES_PER_LINE_int);
            for(int idx=0, xpos=_xPosAscii; idx<ascii.size(); idx++, xpos+=_charWidth)
            {
                QBrush highLighted=QBrush(_highlightingColor);
                painter.setBackground(highLighted);
                painter.setBackgroundMode(Qt::TransparentMode);

                // Highlight selection
                if(selection)
                {
                    int cur_char=lineIdx+idx;
                    if(cur_char>=selection_start && cur_char<=selection_end)
                    {
                        painter.setBackgroundMode(Qt::OpaqueMode);
                    }
                    else
                    {
                        painter.setBackgroundMode(Qt::TransparentMode);
                    }
                }

                // Highlight SEARCH selection
                if(selection_search)
                {
                    int cur_char=lineIdx+idx;
                    if(cur_char >= search_start && cur_char <= serch_end)
                    {
                        painter.setBackgroundMode(Qt::OpaqueMode);
                    }
                    else
                    {
                        painter.setBackgroundMode(Qt::TransparentMode);
                    }
                }

                if(list_struct_indexs.size() > 0)
                {
                    int cur_char = lineIdx+idx;
                    highlight_block_bytes_fun(cur_char,painter);
                }

                painter.setPen(Qt::black);

                // Render char
                if(((char)ascii[idx]<0x20) || ((char)ascii[idx]>0x7e))
                {
                    painter.drawText(xpos, yPos, QString("."));
                }
                else
                {
                    painter.drawText(xpos, yPos, QString(ascii.at(idx)));
                }
            }
        }
    }

    //Reset painter background if it is still set from highlighting
    painter.setBackgroundMode(Qt::TransparentMode);

    // Paint cursor
    if(!this->_data.isNull() && !this->_data.isEmpty())
    {
        if(_overwriteMode)
        {
            painter.fillRect(_cursorX - 2, _cursorY+_charHeight - 2, _charWidth, 4, QBrush(Qt::red));
        }
        else
        {
            painter.fillRect(_cursorX - 2, _cursorY, 2, _charHeight, QBrush(Qt::red));
        }
    }

    if(_size != _data.size())
    {
        _size = _data.size();
        emit currentSizeChanged(_size);
    }


}

void QHexEditPrivate::highlight_block_bytes_fun(int cur_char,QPainter &painter)
{
    for(int i = 0; i < list_struct_indexs.size(); i++)
    {
        uint64_t block_actual_start_in_file = list_struct_indexs.at(i).block_actual_start_offset_in_file;
        uint64_t block_actual_end_in_file = list_struct_indexs.at(i).block_actual_end_offset_in_file;

        uint64_t block_start_index = list_struct_indexs.at(i).start_index;
        uint64_t block_end_offset = list_struct_indexs.at(i).end_index;

        if(((block_actual_start_in_file >= current_page_start_offset) && (block_actual_start_in_file <= current_page_end_offset)) && ((block_actual_end_in_file >= current_page_start_offset) && (block_actual_end_in_file <= current_page_end_offset)))
        {
            if((cur_char >= block_start_index) && (cur_char <= block_end_offset))
            {
                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
                painter.setBackgroundMode(Qt::OpaqueMode);

            }
        }
        else if(((block_actual_start_in_file >= current_page_start_offset) && (block_actual_start_in_file <= current_page_end_offset)) && ((block_actual_end_in_file > current_page_end_offset)))
        {
            if((cur_char >= list_struct_indexs.at(i).start_index) && (cur_char <= _data.size()))
            {
                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
                painter.setBackgroundMode(Qt::OpaqueMode);
            }
        }
        else if((block_actual_end_in_file >= current_page_start_offset) && (block_actual_end_in_file <= current_page_end_offset) && ((block_actual_start_in_file < current_page_start_offset)))
        {
            if((cur_char >= 0 && cur_char) <= list_struct_indexs.at(i).end_index)
            {
                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
                painter.setBackgroundMode(Qt::OpaqueMode);
            }
        }
        else if(((block_actual_start_in_file <= current_page_start_offset) && (block_actual_end_in_file > current_page_start_offset)) && ((block_actual_start_in_file <= current_page_start_offset) && (block_actual_end_in_file > current_page_end_offset)))
        {
            if(cur_char >= 0 && cur_char <= _data.size())
            {
                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
                painter.setBackgroundMode(Qt::OpaqueMode);
            }
        }
        else
        {
            if((cur_char >= block_start_index) && (cur_char <= block_end_offset))
            {
                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
                painter.setBackgroundMode(Qt::OpaqueMode);

            }
        }

    }
}

void QHexEditPrivate::setCursorPos(int position)
{
    // delete cursor
    //    _blink=false;
    update();

    // cursor in range?
    if(_overwriteMode)
    {
        if(position>(_data.size()*2-1)) position=_data.size()*2-1;
    }
    else
    {
        if(position>(_data.size()*2)) position=_data.size()*2;
    }

    if(position < 0) position=0;

    // calc position
    _cursorPosition=position;
    _cursorY=(position/(2*BYTES_PER_LINE_int))*_charHeight+4;
    int x=(position%(2*BYTES_PER_LINE_int));
    _cursorX=(((x/2)*3)+(x%2))*_charWidth+_xPosHex;

    // immiadately draw cursor
    //    _blink=true;
    update();

    emit currentAddressChanged(_cursorPosition/2);
}

void QHexEditPrivate::contextMenuEvent(QContextMenuEvent *p_event)
{
    if(p_event == NULL)
        return;

    // Create context menu and add actions
    QMenu context_menu(this);

    this->p_menu_copy->setEnabled(false);
    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end) || (!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        this->p_menu_copy->setEnabled(true);
    }

    context_menu.addMenu(this->p_menu_copy);
    //    context_menu.addAction(this->p_action_select_all);
    context_menu.addMenu(this->p_menu_change_number_bytes_display);
    context_menu.addMenu(this->p_menu_display_address_in_hex_or_decimal);

    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    context_menu.setFont(m_font);

    this->p_menu_copy->setFont(m_font);
    this->p_menu_tag_selected_bytes->setFont(m_font);
    context_menu.addAction(this->p_action_select_all);

    if(is_context_menu_full)
    {
        context_menu.addAction(this->p_action_tag_start_offset);
        context_menu.addMenu(this->p_menu_tag_end_offset);
        context_menu.addAction(this->p_action_export_selected_bytes);


        this->p_menu_tag_end_offset->setEnabled(false);
        this->p_action_tag_start_offset->setEnabled(true);
        this->p_action_export_selected_bytes->setEnabled(false);

        if(start_offset_clicked)
        {
            this->p_menu_tag_end_offset->setEnabled(true);
            this->p_action_tag_start_offset->setEnabled(false);
        }


    }

    context_menu.addMenu(this->p_menu_tag_selected_bytes);
    this->p_menu_tag_selected_bytes->setEnabled(false);

    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end) || (!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        this->p_action_export_selected_bytes->setEnabled(true);
        this->p_menu_tag_selected_bytes->setEnabled(true);

        if(start_offset_clicked)
        {
            this->p_action_export_selected_bytes->setEnabled(false);
            this->p_menu_tag_selected_bytes->setEnabled(false);
        }
    }

    context_menu.exec(p_event->globalPos());
}

void QHexEditPrivate::updateCursor()
{
    //    if (_blink)
    //        _blink = false;
    //    else
    //        _blink = true;

    update(_cursorX, _cursorY, _charWidth, _charHeight);
}

void QHexEditPrivate::slot_copy_selected_bytes()
{
    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end))
    {
        int selection_start=this->Point2Char(this->sel_start)/2;
        int selection_count=this->Point2Char(this->sel_end)/2;
        selection_count-=(selection_start-1);

        QByteArray hex(this->_data.mid(selection_start,selection_count).toHex());

        QApplication::clipboard()->setText(hex,QClipboard::Clipboard);
    }
    else if((!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        int selection_start=this->Point2Char_for_search(this->search_start)/2;
        int selection_count=this->Point2Char_for_search(this->search_end)/2;
        selection_count-=(selection_start-1);

        QByteArray hex(this->_data.mid(selection_start,selection_count).toHex());

        QApplication::clipboard()->setText(hex,QClipboard::Clipboard);
    }
}

void QHexEditPrivate::slot_copy_selected_text_as_ascii()
{
    if((!(this->sel_start.isNull() && this->sel_end.isNull()) && this->sel_start != this->sel_end))
    {
        int selection_start=this->Point2Char(this->sel_start)/2;
        int selection_count=this->Point2Char(this->sel_end)/2;
        selection_count-=(selection_start-1);

        QByteArray values(this->_data.mid(selection_start,selection_count));

        QString ascii="";
        int i=0;
        for(i=0;i<values.size();i++)
        {
            if(!(((char)values[i]<0x20) || ((char)values[i]>0x7e)))
            {
                ascii.append(values.at(i));
            }
            else
            {
                ascii.append(".");
            }
        }

        QApplication::clipboard()->setText(ascii,QClipboard::Clipboard);
    }
    else if((!(this->search_start.isNull() && this->search_end.isNull()) && this->search_start != this->search_end))
    {
        int selection_start=this->Point2Char_for_search(this->search_start)/2;
        int selection_count=this->Point2Char_for_search(this->search_end)/2;
        selection_count-=(selection_start-1);

        QByteArray values(this->_data.mid(selection_start,selection_count));

        QString ascii="";
        int i=0;
        for(i=0;i<values.size();i++)
        {
            if(!(((char)values[i]<0x20) || ((char)values[i]>0x7e)))
            {
                ascii.append(values.at(i));
            }
            else
            {
                ascii.append(".");
            }
        }

        QApplication::clipboard()->setText(ascii,QClipboard::Clipboard);
    }
}


void QHexEditPrivate::slot_select_all()
{
    this->search_start.setX(0);
    this->search_start.setY(0);

    int y_position=(_size/(BYTES_PER_LINE_int))*_charHeight + 4;

    int tmp = _charWidth*((2*_size)) + _size;								// FINAL

    this->search_end.setX(tmp);
    this->search_end.setY(y_position);

    this->sel_start = this->sel_end = this->sel_origin;

    this->update();
}

void QHexEditPrivate::adjust()
{
    _charWidth = fontMetrics().width(QLatin1Char('9'));
    _charHeight = fontMetrics().height();

    // is addressNumbers wide enough?
    if(bool_is_address_display_in_hex)
    {
        QString test = QString("%1").arg(_data.size() + _addressOffset, _addressNumbers, 16, QChar('0'));
        _realAddressNumbers = test.size();
    }
    else
    {
        QString test = QString("%1").arg(_data.size() + _addressOffset, _addressNumbers, 10, QChar('0'));
        _realAddressNumbers = test.size();
    }

    _xPosAdr = 0;
    if (_addressArea)
        _xPosHex = _realAddressNumbers *_charWidth + GAP_ADR_HEX_int;
    else
        _xPosHex = 0;

    _xPosAscii = _xPosHex + HEXCHARS_IN_LINE_int * _charWidth + GAP_HEX_ASCII_int;

    // tell QAbstractScollbar, how big we are
    setMinimumHeight(((_data.size()/BYTES_PER_LINE_int + 1) * _charHeight) + 3);
    setMinimumWidth(_xPosAscii + (BYTES_PER_LINE_int * _charWidth));

    emit signal_update_column_index_in_upper_label(_charWidth, _charHeight, BYTES_PER_LINE_int, _xPosHex,bool_is_address_display_in_hex);

    update();
}

int QHexEditPrivate::Point2Char(QPoint pos)
{
    // find char under cursor
    if((pos.x()>=_xPosHex) && (pos.x()<(_xPosHex+HEXCHARS_IN_LINE_int*_charWidth)))
    {
        int x=(pos.x()-_xPosHex)/_charWidth;

        if((x%3)==0)
            x=(x/3)*2;
        else
            x=((x/3)*2)+1;

        int y=(pos.y()/_charHeight)*2*BYTES_PER_LINE_int;
        return x+y;
    }
    else if((pos.x()>=_xPosAscii) && (pos.x()<(_xPosAscii+HEXCHARS_IN_LINE_int*_charWidth)))
    {
        int x=((pos.x()-_xPosAscii)/_charWidth) * 3;

        if((x%3)==0)
            x=(x/3)*2;
        else
            x=((x/3)*2)+1;

        int y=(pos.y()/_charHeight)*2*BYTES_PER_LINE_int;
        return x+y;
    }

    return -1;
}

int QHexEditPrivate::Point2Char_for_search(QPoint pos)
{
    // find char under cursor
    if((pos.x()>=_xPosHex))
    {
        int x=(pos.x()-_xPosHex)/_charWidth;

        if((x%3)==0)
            x=(x/3)*2;
        else
            x=((x/3)*2)+1;

        int y=(pos.y()/_charHeight)*2*BYTES_PER_LINE_int;
        return x+y;
    }

    return -1;
}

void QHexEditPrivate::search_hex(QByteArray data, uint64_t page_size, bool is_previous_clicked, bool is_fresh_search)
{

    if(is_previous_clicked || (!bool_show_informative_dialog) || is_fresh_search)
    {
        index_list.clear();
        _cursorPosition = 0;
    }


    if(!index_list.isEmpty())
    {
        if(current_cursor_index < index_list.size())
        {
            emit signal_cursor_current_pos(current_cursor_index);
            highlight_searched_hex_data(data, page_size, current_cursor_index, false);
            current_cursor_index++;
            emit signal_page_end_reached(false);
            return;
        }
        else if(current_cursor_index == index_list.size())
        {
            index_list.clear();
            current_cursor_index = -1;
            emit signal_cursor_current_pos(current_cursor_index);
            emit signal_page_end_reached(true);
            return;
        }
        else
        {

        }
    }

    index_list.clear();
    current_cursor_index = 0;


    data_size = page_size;
    data = data.fromHex(data);
    searched_data_size = data.size();

    QByteArray selected_data_arr = _data.mid(0,page_size);

    if(selected_data_arr.toLower().contains(data.toLower()))
    {
        qint64 start_index = 0,end_index = 0;

        while(start_index != -1)
        {
            start_index = selected_data_arr.toLower().indexOf(data.toLower(),end_index);

            if(start_index >= 0)
            {
                end_index = start_index + data.size();
                index_list << QString::number(start_index);
            }
        }

    }

    if(is_previous_clicked)
        current_cursor_index = QString(index_list.size() -1).toInt();



    highlight_searched_hex_data(data, page_size, current_cursor_index, true);

    current_cursor_index++;
    emit signal_cursor_current_pos(current_cursor_index);
    emit signal_page_end_reached(false);

}

void QHexEditPrivate::highlight_searched_hex_data(QByteArray data,uint64_t page_size , int cursor_pos_index, bool is_fresh_search)
{
    if(!index_list.isEmpty())
    {
        uint64_t position = index_list.at(cursor_pos_index).toULongLong();
        set_cursor_pos_on_searched_text(position, page_size);

        // Highlight Searched Hex
        this->search_start.setX(_cursorX);
        this->search_start.setY(_cursorY);

        int tmp = _charWidth*((2*data.size()) + (data.size() - 1));								// FINAL

        if(!is_fresh_search)
        {
            tmp = _charWidth*data.size();
            tmp += (data.size() - 1);
        }

        this->search_end.setX(_cursorX + tmp);
        this->search_end.setY(_cursorY);

        emit index_list_size(index_list.size());

        // Get values of updated cursor position
        int currr_position = Point2Char_for_search(this->search_start);

        _cursorPosition = currr_position;

        emit current_position_hex(_data.mid(currr_position/2,8));
        emit current_offset(_cursorPosition/2);

    }
    else
    {
        emit index_list_size(index_list.size());

        //        if(bool_show_informative_dialog)
        //        {
        //            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"No result found");
        //            message_dialog_obj->show();
        //        }

    }

}

void QHexEditPrivate::highlight_searched_ascii_text(QString text, uint64_t page_size, int cursor_pos_index)
{

    if(!index_list.isEmpty())
    {
        if(index_list.size() == cursor_pos_index)
            cursor_pos_index = index_list.size() -1;

        uint64_t position = index_list.at(cursor_pos_index).toULongLong();
        set_cursor_pos_on_searched_text(position, page_size);

        // Highlight Searched Hex
        this->search_start.setX(_cursorX);
        this->search_start.setY(_cursorY);

        int tmp = _charWidth*((2*text.size()) + (text.size() - 1));								// FINAL

        this->search_end.setX(_cursorX + tmp);
        this->search_end.setY(_cursorY);

        emit index_list_size(index_list.size());

        // Get values of updated cursor position
        int currr_position = Point2Char_for_search(this->search_start);

        _cursorPosition = currr_position;

        emit current_position_hex(_data.mid(currr_position/2,8));
        emit current_offset(_cursorPosition/2);

    }
    else if(cursor_pos_index == 0)
    {
        uint64_t position = cursor_pos_index;
        set_cursor_pos_on_searched_text(position, page_size);

        // Highlight Searched Hex
        this->search_start.setX(_cursorX);
        this->search_start.setY(_cursorY);

        this->search_end.setX(_cursorX);
        this->search_end.setY(_cursorY);

        emit index_list_size(cursor_pos_index);

        // Get values of updated cursor position
        int currr_position = Point2Char_for_search(this->search_start);

        _cursorPosition = currr_position;

        emit current_position_hex(_data.mid(currr_position/2,8));
        emit current_offset(_cursorPosition/2);

    }
    else
    {
        emit index_list_size(index_list.size());

        //        if(bool_show_informative_dialog)
        //        {
        //            message_dialog_obj->pub_set_message(MACRO_MSG_INFORMATION,"No result found");
        //            message_dialog_obj->show();
        //        }

    }

}

void QHexEditPrivate::search_next(int index, int searched_data_size)
{
    if(index_list.isEmpty())
        return;

    if(index >= index_list.size())
        index = index_list.size() - 1;

    if(index < 0)
        index = 0;

    current_cursor_index = index;

    uint64_t position = index_list.at(index).toULongLong();

    set_cursor_pos_on_searched_text(position, data_size);

    emit signal_cursor_current_pos(index);


    // Highlight Searched Hex
    this->search_start.setX(_cursorX);
    this->search_start.setY(_cursorY);

    int tmp = _charWidth*((2*searched_data_size) + (searched_data_size - 1));								// FINAL

    this->search_end.setX(_cursorX + tmp);
    this->search_end.setY(_cursorY);

    // Get values of updated cursor position
    int curr_position = Point2Char_for_search(this->search_start);

    _cursorPosition = curr_position;

    emit current_position_hex(_data.mid(curr_position/2,8));
    emit current_offset(_cursorPosition/2);
}

void QHexEditPrivate::search_previous(int index, int searched_data_size)
{
    if(index >= index_list.size())
        index = index_list.size() - 1;

    if(index < 0)
        return;

    current_cursor_index = index;

    uint64_t position = index_list.at(index).toULongLong();

    set_cursor_pos_on_searched_text(position, data_size);

    emit signal_cursor_current_pos(index);


    // Highlight Searched Hex
    this->search_start.setX(_cursorX);
    this->search_start.setY(_cursorY);

    int tmp = _charWidth*((2*searched_data_size) + (searched_data_size - 1));								// FINAL

    this->search_end.setX(_cursorX + tmp);
    this->search_end.setY(_cursorY);

    // Get values of updated cursor position
    int curr_position = Point2Char_for_search(this->search_start);

    _cursorPosition = curr_position;

    emit current_position_hex(_data.mid(curr_position/2,8));
    emit current_offset(_cursorPosition/2);
}

void QHexEditPrivate::search_ascii(QString text, uint64_t page_size, bool is_previous_clicked, bool is_fresh_search)
{
    if(is_previous_clicked || (!bool_show_informative_dialog) || is_fresh_search)
    {
        index_list.clear();
        _cursorPosition = 0;
    }

    if(!index_list.isEmpty())
    {
        if(current_cursor_index > index_list.size())
            current_cursor_index = index_list.size() -1;

        if(current_cursor_index < 0)
            current_cursor_index = 0;

        if(current_cursor_index < index_list.size())
        {
            current_cursor_index++;
            emit signal_cursor_current_pos(current_cursor_index);
            highlight_searched_ascii_text(text, page_size,current_cursor_index);
            if(current_cursor_index == index_list.size())
                emit signal_page_end_reached(true);
            else
                emit signal_page_end_reached(false);
            return;
        }
        else if(current_cursor_index == index_list.size())
        {
            index_list.clear();
            current_cursor_index = -1;
            emit signal_cursor_current_pos(current_cursor_index);
            emit signal_page_end_reached(true);
            return;
        }
        else
        {

        }
    }


    index_list.clear();
    current_cursor_index = 0;

    QByteArray values = _data.mid(0, page_size);

    QString ascii="";
    int i=0;
    for(i=0;i<values.size();i++)
    {
        if(!(((char)values[i]<0x20) || ((char)values[i]>0x7e)))
        {
            ascii.append(values.at(i));
        }
        else
        {
            ascii.append(".");
        }
    }


    if(ascii.toLower().contains(text.toLower()))
    {

        qint64 start_index = 0,end_index = 0;

        while(start_index != -1)
        {
            start_index = ascii.toLower().indexOf(text.toLower(),end_index);

            if(start_index >= 0)
            {
                end_index = start_index + text.size();
                index_list << QString::number(start_index);
            }
        }
    }

    //    if(index_list.size() == 0)
    //        return;


    if(is_previous_clicked)
    {
        //        if(index_list.size() ==1)
        //            return;

        current_cursor_index = QString(index_list.size() -1).toInt();
    }


    highlight_searched_ascii_text(text, page_size,current_cursor_index);

    if(index_list.size() > 0)
        current_cursor_index++;

    emit signal_cursor_current_pos(current_cursor_index);
    emit signal_page_end_reached(false);

}


void QHexEditPrivate::set_cursor_pos_on_searched_text(uint64_t position, const uint64_t page_size)
{
    // delete cursor
    //    _blink=false;
    update();

    // cursor in range?
    if(_overwriteMode)
    {
        if(position > (page_size *2 - 1)) position = page_size * 2 - 1;
    }
    else
    {
        if(position>(page_size*2)) position=page_size*2;
    }

    if(position < 0) position=0;

    // calc position														// Final
    _cursorY=(position/(BYTES_PER_LINE_int))*_charHeight+4;
    int x=((2*position)%(2*BYTES_PER_LINE_int));
    _cursorX=(((x/2)*3)+(x%2))*_charWidth+_xPosHex;


    _scrollArea->ensureVisible(_cursorX, _cursorY, 3, 3 + _charHeight);

    // immiadately draw cursor
    //    _blink=true;
    update();

    emit currentAddressChanged(position);
}

void QHexEditPrivate::slot_change_number_bytes_display_submenu_triggered(QAction *action_name)
{
    //    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    QString triggered_name = action_name->text();

    int entered_bytes_size  = 0;
    if(triggered_name == "8 bytes")
        entered_bytes_size = 8;
    else if(triggered_name == "16 bytes")
        entered_bytes_size = 16;
    else if(triggered_name == "32 bytes")
        entered_bytes_size = 32;
    else if(triggered_name == "48 bytes")
        entered_bytes_size = 48;

    int no_of_hexchars_in_a_line = (2 * entered_bytes_size) + (entered_bytes_size - 1);

    HEXCHARS_IN_LINE_int = no_of_hexchars_in_a_line;
    GAP_HEX_ASCII_int = entered_bytes_size;
    BYTES_PER_LINE_int = entered_bytes_size;
    GAP_ADR_HEX_int = 10;

    setCursorPos(0);
    emit current_position_hex(_data.mid(0));
    emit current_offset(0);

    adjust();

    this->update();

    //    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void QHexEditPrivate::slot_action_action_display_address_in_decimal_triggered()
{
    bool_is_address_display_in_hex = false;

    adjust();
    this->update();
}

void QHexEditPrivate::slot_action_action_display_address_in_hex_triggered()
{
    bool_is_address_display_in_hex = true;

    adjust();
    this->update();
}

void QHexEditPrivate::add_actions_in_display_bytes_menu()
{
    QStringList actions_name_list;
    actions_name_list << "8 bytes" << "16 bytes" << "32 bytes" << "48 bytes";

    for(int i = 0; i < actions_name_list.size(); i++)
    {
        QString action_name = actions_name_list.at(i);

        QAction *action = new QAction(tr(action_name.toLocal8Bit().data()),this->p_menu_change_number_bytes_display);
        this->p_menu_change_number_bytes_display->addAction(action);
    }
}


void QHexEditPrivate::go_to_offset(uint64_t position, uint64_t page_size)
{
    // delete cursor
    //    _blink=false;
    update();

    // cursor in range?
    if(_overwriteMode)
    {
        if(position > (page_size *2 - 1)) position = page_size * 2 - 1;
    }
    else
    {
        if(position>(page_size*2)) position=page_size*2;
    }

    if(position < 0) position=0;

    // calc position														// Final
    _cursorY=(position/(BYTES_PER_LINE_int))*_charHeight+4;
    int x=((2*position)%(2*BYTES_PER_LINE_int));
    _cursorX=(((x/2)*3)+(x%2))*_charWidth+_xPosHex;


    _scrollArea->ensureVisible(_cursorX, _cursorY, 3, 3 + _charHeight);


    // immiadately draw cursor
    //    _blink=true;
    update();

    QPoint curr_cursor_point;

    // Get values of updated cursor position
    curr_cursor_point.setX(_cursorX);
    curr_cursor_point.setY(_cursorY);
    int currr_position = Point2Char_for_search(curr_cursor_point);

    this->search_start = this->search_end = this->sel_origin;
    this->sel_end=this->sel_start=this->sel_origin;

    _cursorPosition = currr_position;

    emit current_position_hex(_data.mid(currr_position/2,8));
    emit current_offset(_cursorPosition/2);

    emit currentAddressChanged(position);
}

void QHexEditPrivate::set_searched_data_for_page(int page_size)
{
    this->search_start = this->search_end = this->sel_origin;
    this->sel_end=this->sel_start=this->sel_origin;

    emit current_position_hex(_data.mid(_cursorPosition/2,8));

    index_list.clear();
}

void QHexEditPrivate::clear_block_index_list()
{
    list_struct_indexs.clear();
    this->update();
}

void QHexEditPrivate::set_current_page_details(int page_no,uint64_t page_start_offset,uint64_t page_end_offset)
{
    current_page_no = page_no;
    current_page_start_offset = page_start_offset;
    current_page_end_offset = page_end_offset;
    this->update();
}

void QHexEditPrivate::set_the_list_of_already_set_blocks(QList<struct_global_hex_viewer_start_and_end_index> list)
{
    list_struct_indexs = list;
    this->update();
}

void QHexEditPrivate::set_context_menu(bool val)
{
    is_context_menu_full = val;
}

void QHexEditPrivate::set_block_db(QString db_path)
{
    block_db_path = db_path;
}
