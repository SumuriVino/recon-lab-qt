#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QPainter>
#include <QScrollBar>

#include "qhexedit_hexedit.h"

// ********************************************************************** Constructor, destructor

QHexEdit_hexedit::QHexEdit_hexedit(QWidget *parent) : QAbstractScrollArea(parent)
{
    _addressArea = true;
    _addressWidth = 4;
    _asciiArea = true;
    _overwriteMode = true;
    _highlighting = true;
    _readOnly = false;
    _cursorPosition = 0;
    _lastEventSize = 0;
    _hexCharsInLine = 47;
    _bytesPerLine = 16;
    _editAreaIsAscii = false;
    _hexCaps = true;
    _dynamicBytesPerLine = false;
    bool_show_offsets_in_decimal = false;

    _chunks = new Chunks_hexedit(this);
    _undoStack = new UndoStack(_chunks, this);
#ifdef Q_OS_WIN32
    setFont(QFont("Courier", 10));
#else
    setFont(QFont("Courier", 13));
#endif
    setAddressAreaColor(this->palette().alternateBase().color());
    setHighlightingColor(QColor(0xff, 0xff, 0x99, 0xff));
    //setHighlightingColor(QColor("blue"));
    setSelectionColor(QColor("blue"));
    //setSelectionColor(this->palette().highlight().color());

    connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursor()));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust()));
    connect(_undoStack, SIGNAL(indexChanged(int)), this, SLOT(dataChangedPrivate(int)));

    bool_change_cursor_pos = false;

    verticalScrollBar()->hide();

    _cursorTimer.setInterval(500);
    _cursorTimer.start();

    setAddressWidth(4);
    setAddressArea(true);
    setAsciiArea(true);
    setOverwriteMode(true);
    setHighlighting(true);
    setReadOnly(false);

    init();

}

QHexEdit_hexedit::~QHexEdit_hexedit()
{
}

// ********************************************************************** Properties

void QHexEdit_hexedit::setAddressArea(bool addressArea)
{
    _addressArea = addressArea;
    adjust();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

bool QHexEdit_hexedit::addressArea()
{
    return _addressArea;
}

void QHexEdit_hexedit::setAddressAreaColor(const QColor &color)
{
    _addressAreaColor = color;
    viewport()->update();
}

QColor QHexEdit_hexedit::addressAreaColor()
{
    return _addressAreaColor;
}

void QHexEdit_hexedit::setAddressOffset(qint64 addressOffset)
{
    _addressOffset = addressOffset;
    adjust();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

qint64 QHexEdit_hexedit::addressOffset()
{
    return _addressOffset;
}

void QHexEdit_hexedit::setAddressWidth(int addressWidth)
{
    _addressWidth = addressWidth;
    adjust();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

int QHexEdit_hexedit::addressWidth()
{
    qint64 size = _chunks->size();
    int n = 1;
    //    if (size > Q_INT64_C(0x100000000)){ n += 8; size /= Q_INT64_C(0x100000000);}
    //    if (size > 0x10000){ n += 4; size /= 0x10000;}
    //    if (size > 0x100){ n += 2; size /= 0x100;}
    //    if (size > 0x10){ n += 1;}

    // -- Manoj
    //    int n = 1;
    if (size > Q_INT64_C(0x100000000)){ n += 8; size /= Q_INT64_C(0x100000000);}
    if (size > 0x10000){ n += 4; size /= 0x10000;}
    if (size > 0x100){ n += 4; size /= 0x100;}
    if (size > 0x10){ n += 4;}

    if (n > _addressWidth)
        return n;
    else
        return _addressWidth;
}

void QHexEdit_hexedit::setAsciiArea(bool asciiArea)
{
    if (!asciiArea)
        _editAreaIsAscii = false;
    _asciiArea = asciiArea;
    adjust();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

bool QHexEdit_hexedit::asciiArea()
{
    return _asciiArea;
}

void QHexEdit_hexedit::setBytesPerLine(int count)
{
    _bytesPerLine = count;
    _hexCharsInLine = count * 3 - 1;

    adjust();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

int QHexEdit_hexedit::bytesPerLine()
{
    return _bytesPerLine;
}

void QHexEdit_hexedit::setCursorPosition(qint64 position)
{
    emit signal_cursor_position(position / 2);

    // 1. delete old cursor
    _blink = false;
    viewport()->update(_cursorRect);

    // 2. Check, if cursor in range?
    if (position > (_chunks->size() * 2 - 1))
        position = _chunks->size() * 2  - (_overwriteMode ? 1 : 0);

    if (position < 0)
        position = 0;

    // 3. Calc new position of cursor
    _bPosCurrent = position / 2;
    _pxCursorY = ((position / 2 - _bPosFirst) / _bytesPerLine + 1) * _pxCharHeight;

    _pxCursorY = _pxCursorY + _pxCharHeight + _pxCharHeight/2; // -- Manoj
    int x = (position % (2 * _bytesPerLine));
    if (_editAreaIsAscii)
    {
        _pxCursorX = x / 2 * _pxCharWidth + _pxPosAsciiX;
        _cursorPosition = position & 0xFFFFFFFFFFFFFFFE;
    } else {
        _pxCursorX = (((x / 2) * 3) + (x % 2)) * _pxCharWidth + _pxPosHexX;
        _cursorPosition = position;
    }

    if (_overwriteMode)
        _cursorRect = QRect(_pxCursorX - horizontalScrollBar()->value(), _pxCursorY + _pxCursorWidth, _pxCharWidth, _pxCursorWidth);
    else
        _cursorRect = QRect(_pxCursorX - horizontalScrollBar()->value(), _pxCursorY - _pxCharHeight + 4, _pxCursorWidth, _pxCharHeight);

    // 4. Immediately draw new cursor
    _blink = true;
    viewport()->update(_cursorRect);
    emit currentAddressChanged(_bPosCurrent);

}

qint64 QHexEdit_hexedit::cursorPosition(QPoint pos)
{
    // Calc cursor position depending on a graphical position
    qint64 result = -1;
    int posX = pos.x() + horizontalScrollBar()->value();
    int posY = pos.y() - 3 -_pxCharHeight -_pxCharHeight/2;
    // int posY = pos.y() - 3; -- Manoj

    //    if(posX <= _pxPosHexX)
    //        bool_show_offsets_in_decimal = !bool_show_offsets_in_decimal;

    if ((posX >= _pxPosHexX) && (posX < (_pxPosHexX + (1 + _hexCharsInLine) * _pxCharWidth)))
    {
        _editAreaIsAscii = false;
        int x = (posX - _pxPosHexX) / _pxCharWidth;
        x = (x / 3) * 2 + x % 3;
        int y = (posY / _pxCharHeight) * 2 * _bytesPerLine ;
        result = _bPosFirst * 2 + x + y;
    }
    else
        if (_asciiArea && (posX >= _pxPosAsciiX) && (posX < (_pxPosAsciiX + (1 + _bytesPerLine) * _pxCharWidth)))
        {
            _editAreaIsAscii = true;
            int x = 2 * (posX - _pxPosAsciiX) / _pxCharWidth;
            int y = (posY / _pxCharHeight) * 2 * _bytesPerLine ;
            result = _bPosFirst * 2 + x + y;
        }
    return result;
}

qint64 QHexEdit_hexedit::cursorPosition()
{
    return _cursorPosition;
}

bool QHexEdit_hexedit::pub_is_data_available()
{
    if(source_file_size > 0)
        return true;

    return false;
}

void QHexEdit_hexedit::pub_set_64_bit_scroll_bar_obj(Scrollbar64 *obj)
{
    scroll_bar_64_obj = obj;
    //init();
    scroll_bar_64_obj->SetCurPos(0);
    connect(scroll_bar_64_obj, SIGNAL(ScrollbarChanged(qint64)), this, SLOT(slot_scroll_bar_64_changed(qint64)));
}

void QHexEdit_hexedit::setData(const QByteArray &ba)
{
    _data = ba;


    _bData.setData(_data);
    setData(_bData);
}

QByteArray QHexEdit_hexedit::data()
{
    return _chunks->data(0, -1);
}

void QHexEdit_hexedit::setHighlighting(bool highlighting)
{
    _highlighting = highlighting;
    viewport()->update();
}

bool QHexEdit_hexedit::highlighting()
{
    return _highlighting;
}

void QHexEdit_hexedit::wheelEvent(QWheelEvent *event)
{
    bool_change_cursor_pos = true;

    QPoint point = event->angleDelta();
    if(point.y() > 0)
    {
        if(_cursorPosition == 0)
            return;

        qint64 value = 0;
        if(scroll_bar_64_obj != NULL)
        {
            qint64 adr = ((_cursorPosition - (((_rowsShown - 1) * _bytesPerLine))) / bytesPerLine());
            scroll_bar_64_obj->SetCurPos(adr);
            scroll_bar_64_obj->update();

        }
        qint64 cur_pos = (_cursorPosition - (((_rowsShown - 1) * 2 * _bytesPerLine)));

        if(cur_pos < 0)
            cur_pos = 0;

        setCursorPosition(cur_pos);
        resetSelection(_cursorPosition);
        refresh();
    }
    else
    {
        if(scroll_bar_64_obj != NULL)
        {
            qint64 adr = ((_cursorPosition + (((_rowsShown - 1) * _bytesPerLine))) / bytesPerLine());
            scroll_bar_64_obj->SetCurPos(adr);
            scroll_bar_64_obj->update();
        }
        qint64 cur_pos = (_cursorPosition + (((_rowsShown - 1) * 2 * _bytesPerLine)));

        if(cur_pos > (_bPosLast * 2))
        {
            cur_pos = _bPosLast * 2;
        }

        setCursorPosition(cur_pos);
        resetSelection(_cursorPosition);
        refresh();
    }

    bool_change_cursor_pos = false;
}

void QHexEdit_hexedit::setHighlightingColor(const QColor &color)
{
    _brushHighlighted = QBrush(color);
    _penHighlighted = QPen(viewport()->palette().color(QPalette::WindowText));
    viewport()->update();
}

QColor QHexEdit_hexedit::highlightingColor()
{
    return _brushHighlighted.color();
}

void QHexEdit_hexedit::setOverwriteMode(bool overwriteMode)
{
    _overwriteMode = overwriteMode;
    emit overwriteModeChanged(overwriteMode);
}

bool QHexEdit_hexedit::overwriteMode()
{
    return _overwriteMode;
}

void QHexEdit_hexedit::setSelectionColor(const QColor &color)
{
    _brushSelection = QBrush(color);
    _penSelection = QPen(Qt::white);
    viewport()->update();
}

qint64 QHexEdit_hexedit::pub_get_selection_start()
{
    return _bSelectionEnd;
}

qint64 QHexEdit_hexedit::pub_get_selection_end()
{
    return _bSelectionBegin;
}

void QHexEdit_hexedit::pub_set_jump_to_address(qint64 adr)
{
    if(adr < 0)
        adr = 0;

    // verticalScrollBar()->setValue(static_cast<int>(adr) / bytesPerLine());

    if(scroll_bar_64_obj != NULL)
    {
        scroll_bar_64_obj->SetCurPos(adr / bytesPerLine());
    }

    setCursorPosition(adr*2);
}

qint64 QHexEdit_hexedit::pub_get_current_address()
{
    if(scroll_bar_64_obj != NULL)
    {
        return scroll_bar_64_obj->value() * bytesPerLine();
    }

    return 0;

    //return verticalScrollBar()->value() * bytesPerLine();
}

void QHexEdit_hexedit::pub_refresh_view()
{
    adjust();
    viewport()->update();
}

void QHexEdit_hexedit::pub_set_source_file(QString path)
{
    sourse_file_path = path;
    _chunks->pub_set_source_file_path(sourse_file_path);
}

void QHexEdit_hexedit::pub_set_source_size(qint64 size)
{
    source_file_size = size;
    _chunks->pub_set_source_size(size);
}

void QHexEdit_hexedit::pub_set_source_block_size(int size)
{
    source_block_size = size;
    _chunks->pub_set_source_block_size(size);
}

void QHexEdit_hexedit::pub_set_hightlight_tags_list(QList<struct_hex_viewer_tag> list)
{
    highlight_tags_list = list;
}



QColor QHexEdit_hexedit::selectionColor()
{
    return _brushSelection.color();
}

bool QHexEdit_hexedit::isReadOnly()
{
    return _readOnly;
}

void QHexEdit_hexedit::setReadOnly(bool readOnly)
{
    _readOnly = readOnly;
}

void QHexEdit_hexedit::setHexCaps(const bool isCaps)
{
    if (_hexCaps != isCaps)
    {
        _hexCaps = isCaps;
        _hexCaps = true;
        viewport()->update();
    }
}

bool QHexEdit_hexedit::hexCaps()
{
    return _hexCaps;
}

void QHexEdit_hexedit::setDynamicBytesPerLine(const bool isDynamic)
{
    _dynamicBytesPerLine = isDynamic;
    resizeEvent(NULL);
}

bool QHexEdit_hexedit::dynamicBytesPerLine()
{
    return _dynamicBytesPerLine;
}

// ********************************************************************** Access to data of qhexedit
bool QHexEdit_hexedit::setData(QIODevice &iODevice)
{
    bool ok = _chunks->setIODevice(iODevice);
    init();
    dataChangedPrivate();
    return ok;
}

QByteArray QHexEdit_hexedit::dataAt(qint64 pos, qint64 count)
{
    return _chunks->data(pos, count);
}

bool QHexEdit_hexedit::write(QIODevice &iODevice, qint64 pos, qint64 count)
{
    return _chunks->write(iODevice, pos, count);
}

// ********************************************************************** Char handling
void QHexEdit_hexedit::insert(qint64 index, char ch)
{
    _undoStack->insert(index, ch);
    refresh();
}

void QHexEdit_hexedit::remove(qint64 index, qint64 len)
{
    _undoStack->removeAt(index, len);
    refresh();
}

void QHexEdit_hexedit::replace(qint64 index, char ch)
{
    _undoStack->overwrite(index, ch);
    refresh();
}

// ********************************************************************** ByteArray handling
void QHexEdit_hexedit::insert(qint64 pos, const QByteArray &ba)
{
    _undoStack->insert(pos, ba);
    refresh();
}

void QHexEdit_hexedit::replace(qint64 pos, qint64 len, const QByteArray &ba)
{
    _undoStack->overwrite(pos, len, ba);
    refresh();
}

// ********************************************************************** Utility functions
void QHexEdit_hexedit::ensureVisible()
{
    //    if (_cursorPosition < (_bPosFirst * 2))
    //        verticalScrollBar()->setValue(static_cast<int>(_cursorPosition / 2 / _bytesPerLine));
    //    if (_cursorPosition > ((_bPosFirst + (_rowsShown - 1)*_bytesPerLine) * 2))
    //        verticalScrollBar()->setValue(static_cast<int>(_cursorPosition / 2 / _bytesPerLine) - _rowsShown + 1);

    if (_cursorPosition < (_bPosFirst * 2))
        scroll_bar_64_obj->SetCurPos((_cursorPosition / 2 / _bytesPerLine));

    if (_cursorPosition > ((_bPosFirst + (_rowsShown - 1)*_bytesPerLine) * 2))
        scroll_bar_64_obj->SetCurPos((_cursorPosition / 2 / _bytesPerLine) - _rowsShown + 1);

    if (_pxCursorX < horizontalScrollBar()->value())
        horizontalScrollBar()->setValue(_pxCursorX);
    if ((_pxCursorX + _pxCharWidth) > (horizontalScrollBar()->value() + viewport()->width()))
        horizontalScrollBar()->setValue(_pxCursorX + _pxCharWidth - viewport()->width());
    viewport()->update();
}

qint64 QHexEdit_hexedit::indexOf(const QByteArray &ba, qint64 from)
{
    qint64 pos = _chunks->indexOf(ba, from);
    if (pos > -1)
    {
        qint64 curPos = pos*2;
        // setCursorPosition(curPos + ba.length()*2);
        setCursorPosition(curPos);
        resetSelection(curPos);
        setSelection(curPos + ba.length()*2);
        ensureVisible();
    }
    return pos;
}

bool QHexEdit_hexedit::isModified()
{
    return _modified;
}

qint64 QHexEdit_hexedit::lastIndexOf(const QByteArray &ba, qint64 from)
{
    qint64 pos = _chunks->lastIndexOf(ba, from);
    if (pos > -1)
    {
        qint64 curPos = pos*2;
        setCursorPosition(curPos - 1);
        resetSelection(curPos);
        setSelection(curPos + ba.length()*2);
        ensureVisible();
    }
    return pos;
}

void QHexEdit_hexedit::redo()
{
    _undoStack->redo();
    setCursorPosition(_chunks->pos()*(_editAreaIsAscii ? 1 : 2));
    refresh();
}

QString QHexEdit_hexedit::selectionToReadableString()
{
    QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
    return toReadable(ba);
}

QString QHexEdit_hexedit::selectedData()
{
    QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin()).toHex();
    return ba;
}

void QHexEdit_hexedit::setFont(const QFont &font)
{
    QWidget::setFont(font);
    _pxCharWidth = fontMetrics().horizontalAdvance(QLatin1Char('M'));
    _pxCharHeight = fontMetrics().height();
    _pxGapAdr = _pxCharWidth / 2;
    _pxGapAdrHex = 6 * _pxCharWidth;
    _pxGapHexAscii = 2 * _pxCharWidth;
    _pxCursorWidth = _pxCharHeight / 7;
    _pxSelectionSub = _pxCharHeight / 5;
    viewport()->update();
}

QString QHexEdit_hexedit::toReadableString()
{
    QByteArray ba = _chunks->data();
    return toReadable(ba);
}

void QHexEdit_hexedit::pub_set_tags_list_obj(QList<struct_hex_viewer_tag> *list)
{
    tags_list = list;
}

void QHexEdit_hexedit::undo()
{
    _undoStack->undo();
    setCursorPosition(_chunks->pos()*(_editAreaIsAscii ? 1 : 2));
    refresh();
}

// ********************************************************************** Handle events
void QHexEdit_hexedit::keyPressEvent(QKeyEvent *event)
{
    // Cursor movements
    if (event->matches(QKeySequence::MoveToNextChar))
    {
        qint64 pos = _cursorPosition + 1;
        if (_editAreaIsAscii)
            pos += 1;
        setCursorPosition(pos);
        resetSelection(pos);
    }
    if (event->matches(QKeySequence::MoveToPreviousChar))
    {
        qint64 pos = _cursorPosition - 1;
        if (_editAreaIsAscii)
            pos -= 1;
        setCursorPosition(pos);
        resetSelection(pos);
    }
    if (event->matches(QKeySequence::MoveToEndOfLine))
    {
        qint64 pos = _cursorPosition - (_cursorPosition % (2 * _bytesPerLine)) + (2 * _bytesPerLine) - 1;
        setCursorPosition(pos);
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToStartOfLine))
    {
        qint64 pos = _cursorPosition - (_cursorPosition % (2 * _bytesPerLine));
        setCursorPosition(pos);
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToPreviousLine))
    {
        setCursorPosition(_cursorPosition - (2 * _bytesPerLine));
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToNextLine))
    {
        setCursorPosition(_cursorPosition + (2 * _bytesPerLine));
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToNextPage))
    {
        setCursorPosition(_cursorPosition + (((_rowsShown - 1) * 2 * _bytesPerLine)));
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToPreviousPage))
    {
        setCursorPosition(_cursorPosition - (((_rowsShown - 1) * 2 * _bytesPerLine)));
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToEndOfDocument))
    {
        setCursorPosition(_chunks->size() * 2 );
        resetSelection(_cursorPosition);
    }
    if (event->matches(QKeySequence::MoveToStartOfDocument))
    {
        setCursorPosition(0);
        resetSelection(_cursorPosition);
    }

    // Select commands
    if (event->matches(QKeySequence::SelectAll))
    {
        resetSelection(0);
        setSelection(2 * _chunks->size() + 1);
    }
    if (event->matches(QKeySequence::SelectNextChar))
    {
        qint64 pos = _cursorPosition + 1;
        if (_editAreaIsAscii)
            pos += 1;
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectPreviousChar))
    {
        qint64 pos = _cursorPosition - 1;
        if (_editAreaIsAscii)
            pos -= 1;
        setSelection(pos);
        setCursorPosition(pos);
    }
    if (event->matches(QKeySequence::SelectEndOfLine))
    {
        qint64 pos = _cursorPosition - (_cursorPosition % (2 * _bytesPerLine)) + (2 * _bytesPerLine) - 1;
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectStartOfLine))
    {
        qint64 pos = _cursorPosition - (_cursorPosition % (2 * _bytesPerLine));
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectPreviousLine))
    {
        qint64 pos = _cursorPosition - (2 * _bytesPerLine);
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectNextLine))
    {
        qint64 pos = _cursorPosition + (2 * _bytesPerLine);
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectNextPage))
    {
        qint64 pos = _cursorPosition + (((viewport()->height() / _pxCharHeight) - 1) * 2 * _bytesPerLine);
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectPreviousPage))
    {
        qint64 pos = _cursorPosition - (((viewport()->height() / _pxCharHeight) - 1) * 2 * _bytesPerLine);
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectEndOfDocument))
    {
        qint64 pos = _chunks->size() * 2;
        setCursorPosition(pos);
        setSelection(pos);
    }
    if (event->matches(QKeySequence::SelectStartOfDocument))
    {
        qint64 pos = 0;
        setCursorPosition(pos);
        setSelection(pos);
    }

    // Edit Commands
    if (!_readOnly)
    {
        /* Cut */
        if (event->matches(QKeySequence::Cut))
        {
            QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin()).toHex();
            for (qint64 idx = 32; idx < ba.size(); idx +=33)
                ba.insert(idx, "\n");
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(ba);
            if (_overwriteMode)
            {
                qint64 len = getSelectionEnd() - getSelectionBegin();
                replace(getSelectionBegin(), (int)len, QByteArray((int)len, char(0)));
            }
            else
            {
                remove(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
            }
            setCursorPosition(2 * getSelectionBegin());
            resetSelection(2 * getSelectionBegin());
        } else

            /* Paste */
            if (event->matches(QKeySequence::Paste))
            {
                QClipboard *clipboard = QApplication::clipboard();
                QByteArray ba = QByteArray().fromHex(clipboard->text().toLatin1());
                if (_overwriteMode)
                {
                    ba = ba.left(std::min<qint64>(ba.size(), (_chunks->size() - _bPosCurrent)));
                    replace(_bPosCurrent, ba.size(), ba);
                }
                else
                    insert(_bPosCurrent, ba);
                setCursorPosition(_cursorPosition + 2 * ba.size());
                resetSelection(getSelectionBegin());
            } else

                /* Delete char */
                if (event->matches(QKeySequence::Delete))
                {
                    if (getSelectionBegin() != getSelectionEnd())
                    {
                        _bPosCurrent = getSelectionBegin();
                        if (_overwriteMode)
                        {
                            QByteArray ba = QByteArray(getSelectionEnd() - getSelectionBegin(), char(0));
                            replace(_bPosCurrent, ba.size(), ba);
                        }
                        else
                        {
                            remove(_bPosCurrent, getSelectionEnd() - getSelectionBegin());
                        }
                    }
                    else
                    {
                        if (_overwriteMode)
                            replace(_bPosCurrent, char(0));
                        else
                            remove(_bPosCurrent, 1);
                    }
                    setCursorPosition(2 * _bPosCurrent);
                    resetSelection(2 * _bPosCurrent);
                } else

                    /* Backspace */
                    if ((event->key() == Qt::Key_Backspace) && (event->modifiers() == Qt::NoModifier))
                    {
                        if (getSelectionBegin() != getSelectionEnd())
                        {
                            _bPosCurrent = getSelectionBegin();
                            setCursorPosition(2 * _bPosCurrent);
                            if (_overwriteMode)
                            {
                                QByteArray ba = QByteArray(getSelectionEnd() - getSelectionBegin(), char(0));
                                replace(_bPosCurrent, ba.size(), ba);
                            }
                            else
                            {
                                remove(_bPosCurrent, getSelectionEnd() - getSelectionBegin());
                            }
                            resetSelection(2 * _bPosCurrent);
                        }
                        else
                        {
                            bool behindLastByte = false;
                            if ((_cursorPosition / 2) == _chunks->size())
                                behindLastByte = true;

                            _bPosCurrent -= 1;
                            if (_overwriteMode)
                                replace(_bPosCurrent, char(0));
                            else
                                remove(_bPosCurrent, 1);

                            if (!behindLastByte)
                                _bPosCurrent -= 1;

                            setCursorPosition(2 * _bPosCurrent);
                            resetSelection(2 * _bPosCurrent);
                        }
                    } else

                        /* undo */
                        if (event->matches(QKeySequence::Undo))
                        {
                            undo();
                        } else

                            /* redo */
                            if (event->matches(QKeySequence::Redo))
                            {
                                redo();
                            } else

                                if ((QApplication::keyboardModifiers() == Qt::NoModifier) ||
                                        (QApplication::keyboardModifiers() == Qt::KeypadModifier) ||
                                        (QApplication::keyboardModifiers() == Qt::ShiftModifier) ||
                                        (QApplication::keyboardModifiers() == (Qt::AltModifier | Qt::ControlModifier)) ||
                                        (QApplication::keyboardModifiers() == Qt::GroupSwitchModifier))
                                {
                                    /* Hex and ascii input */
                                    int key;
                                    if (_editAreaIsAscii)
                                        key = (uchar)event->text()[0].toLatin1();
                                    else
                                        key = int(event->text()[0].toLower().toLatin1());

                                    if ((((key >= '0' && key <= '9') || (key >= 'a' && key <= 'f')) && _editAreaIsAscii == false)
                                            || (key >= ' ' && _editAreaIsAscii))
                                    {
                                        if (getSelectionBegin() != getSelectionEnd())
                                        {
                                            if (_overwriteMode)
                                            {
                                                qint64 len = getSelectionEnd() - getSelectionBegin();
                                                replace(getSelectionBegin(), (int)len, QByteArray((int)len, char(0)));
                                            } else
                                            {
                                                remove(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
                                                _bPosCurrent = getSelectionBegin();
                                            }
                                            setCursorPosition(2 * _bPosCurrent);
                                            resetSelection(2 * _bPosCurrent);
                                        }

                                        // If insert mode, then insert a byte
                                        if (_overwriteMode == false)
                                            if ((_cursorPosition % 2) == 0)
                                                insert(_bPosCurrent, char(0));

                                        // Change content
                                        if (_chunks->size() > 0)
                                        {
                                            char ch = key;
                                            if (!_editAreaIsAscii){
                                                QByteArray hexValue = _chunks->data(_bPosCurrent, 1).toHex();
                                                if ((_cursorPosition % 2) == 0)
                                                    hexValue[0] = key;
                                                else
                                                    hexValue[1] = key;
                                                ch = QByteArray().fromHex(hexValue)[0];
                                            }
                                            replace(_bPosCurrent, ch);
                                            if (_editAreaIsAscii)
                                                setCursorPosition(_cursorPosition + 2);
                                            else
                                                setCursorPosition(_cursorPosition + 1);
                                            resetSelection(_cursorPosition);
                                        }
                                    }
                                }


    }

    /* Copy */
    if (event->matches(QKeySequence::Copy))
    {
        QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin()).toHex();
        for (qint64 idx = 32; idx < ba.size(); idx +=33)
            ba.insert(idx, "\n");
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ba);
    }

    // Switch between insert/overwrite mode
    if ((event->key() == Qt::Key_Insert) && (event->modifiers() == Qt::NoModifier))
    {
        setOverwriteMode(!overwriteMode());
        setCursorPosition(_cursorPosition);
    }

    // switch from hex to ascii edit
    if (event->key() == Qt::Key_Tab && !_editAreaIsAscii){
        _editAreaIsAscii = true;
        setCursorPosition(_cursorPosition);
    }

    // switch from ascii to hex edit
    if (event->key() == Qt::Key_Backtab  && _editAreaIsAscii){
        _editAreaIsAscii = false;
        setCursorPosition(_cursorPosition);
    }

    refresh();
}

void QHexEdit_hexedit::mouseMoveEvent(QMouseEvent * event)
{
    _blink = false;
    viewport()->update();
    qint64 actPos = cursorPosition(event->pos());
    if (actPos >= 0)
    {
        setCursorPosition(actPos + 2);
        setSelection(actPos + 2);
    }

}

void QHexEdit_hexedit::mousePressEvent(QMouseEvent * event)
{

    if(event->button() == Qt::RightButton)
    {
        emit signal_right_click();

    }

    if(event->button() == Qt::LeftButton)
    {
        highlight_tags_list.clear();

        emit signal_left_click();

        int posX = event->pos().x() + horizontalScrollBar()->value();
        if(posX <= _pxPosHexX)
            bool_show_offsets_in_decimal = !bool_show_offsets_in_decimal;


    }


    _blink = false;
    viewport()->update();
    qint64 cPos = cursorPosition(event->pos());
    if (cPos >= 0)
    {
        if (event->button() != Qt::RightButton)
            resetSelection(cPos);
        setCursorPosition(cPos);
    }


    qint64 mPos;
    if(cPos % 2 == 0)
    {
        mPos = cPos / 2;
    }
    else
    {
        mPos = cPos / 2;
        //mPos = cPos / 2 + 1;
    }

    emit signal_cursor_position(mPos);
}

void QHexEdit_hexedit::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    int pxOfsX = horizontalScrollBar()->value();

    if (event->rect() != _cursorRect)
    {
        int pxPosStartY = _pxCharHeight + _pxCharHeight + _pxCharHeight/2; //-- Manoj
        // int pxPosStartY = _pxCharHeight;

        // draw some patterns if needed
        painter.fillRect(event->rect(), viewport()->palette().color(QPalette::Base));
        //painter.fillRect(event->rect(), viewport()->palette().color(QPalette::AlternateBase));

        if (_addressArea)
        {
            painter.fillRect(QRect(-pxOfsX, event->rect().top(), _pxPosHexX - _pxGapAdrHex/2, height()), _addressAreaColor);
        }
        if (_asciiArea)
        {
            int linePos = _pxPosAsciiX - (_pxGapHexAscii / 2);
            painter.setPen(Qt::gray);
            painter.drawLine(linePos - pxOfsX, event->rect().top(), linePos - pxOfsX, height());
        }

        painter.setPen(viewport()->palette().color(QPalette::WindowText));

        // paint address area
        if (_addressArea)
        {
            painter.setPen(Qt::darkMagenta);
            painter.drawText(_pxPosAdrX - pxOfsX, _pxCharHeight, QString("Offset"));


            QString address;
            for (int row=0, pxPosY = _pxCharHeight + _pxCharHeight  + _pxCharHeight/2; row <= (_dataShown.size()/_bytesPerLine); row++, pxPosY +=_pxCharHeight)
                //               for (int row=0, pxPosY = _pxCharHeight; row <= (_dataShown.size()/_bytesPerLine); row++, pxPosY +=_pxCharHeight)
            {
                if(bool_show_offsets_in_decimal)
                    address = QString("%1").arg(_bPosFirst + row*_bytesPerLine + _addressOffset, _addrDigits, 16, QChar('0'));
                else
                    address = QString("%1").arg(_bPosFirst + row*_bytesPerLine + _addressOffset, _addrDigits, 10, QChar('0'));
                painter.drawText(_pxPosAdrX - pxOfsX, pxPosY, address);
            }
        }
        painter.setPen(viewport()->palette().color(QPalette::WindowText));

        // paint hex and ascii area
        QPen colStandard = QPen(viewport()->palette().color(QPalette::WindowText));

        painter.setBackgroundMode(Qt::TransparentMode);
        int t_pxPosX = _pxPosHexX  - pxOfsX;

        for(int m_col = 0; m_col < _bytesPerLine ; m_col++)
        {
            QByteArray t_ar;
            t_ar.append(m_col);
            painter.setPen(Qt::darkMagenta);
            if(!bool_show_offsets_in_decimal)
                painter.drawText(t_pxPosX, _pxCharHeight, QString::number(t_ar.toHex().toInt(0, 16)));
            else
                painter.drawText(t_pxPosX, _pxCharHeight, t_ar.toHex());


            t_pxPosX += 3*_pxCharWidth;
        }

        painter.drawLine(pxOfsX, _pxCharHeight + 5, pxOfsX + width(), _pxCharHeight + 5);

        for (int row = 0, pxPosY = pxPosStartY ; row <= _rowsShown; row++, pxPosY +=_pxCharHeight)
        {
            // Experiment
            ///
            QByteArray hex;
            int pxPosX = _pxPosHexX  - pxOfsX;
            int pxPosAsciiX2 = _pxPosAsciiX  - pxOfsX;
            qint64 bPosLine = row * _bytesPerLine;


            QColor row_color;

            //            if(row % 2)
            //                row_color = QColor("#cccccc");
            //            else
            //                row_color = QColor("#FFFFFF");


            for (int colIdx = 0; ((bPosLine + colIdx) < _dataShown.size() && (colIdx < _bytesPerLine)); colIdx++)
            {
                QColor c = viewport()->palette().color(QPalette::Base);
                //QColor c = row_color;
                painter.setPen(colStandard);

                qint64 posBa = _bPosFirst + bPosLine + colIdx;


                //                if(((posBa/512) % 2) == 0)
                //                {
                //                    c = QColor("#d2d2d2");
                //                    //painter.setPen(_penSelection);
                //                }

                if ((getSelectionBegin() <= posBa) && (getSelectionEnd() > posBa))
                {
                    c = _brushSelection.color();
                    painter.setPen(_penSelection);
                }
                else if( highlight_block_bytes_fun(posBa, c) ) /// MANOJ ---- SAMPLE highlight ------ IMPORTANT
                {
                    //c = QColor("red");

                    double luma = ((0.299 * c.red()) + (0.587 * c.green()) + (0.114 * c.blue())) / 255;

                    if(luma < 0.5)
                        painter.setPen(_penSelection);

                    // Return black for bright colors, white for dark colors
                    //return luma > 0.5 ? Color.Black : Color.White;
                }
                else
                {
                    if (_highlighting)
                        if (_markedShown.at((int)(posBa - _bPosFirst)))
                        {
                            c = _brushHighlighted.color();
                            painter.setPen(_penHighlighted);
                        }
                }

                // render hex value
                QRect r;
                if (colIdx == 0)
                    r.setRect(pxPosX, pxPosY - _pxCharHeight + _pxSelectionSub, 2*_pxCharWidth, _pxCharHeight);
                else
                    r.setRect(pxPosX - _pxCharWidth, pxPosY - _pxCharHeight + _pxSelectionSub, 3*_pxCharWidth, _pxCharHeight);
                painter.fillRect(r, c);
                hex = _hexDataShown.mid((bPosLine + colIdx) * 2, 2);
                painter.drawText(pxPosX, pxPosY, hexCaps()?hex.toUpper():hex);
                pxPosX += 3*_pxCharWidth;

                // render ascii value
                if (_asciiArea)
                {
                    int ch = (uchar)_dataShown.at(bPosLine + colIdx);


                    if ( (ch < ' ' || ch > '~') && ch != '_' )
                        ch = '.';
                    r.setRect(pxPosAsciiX2, pxPosY - _pxCharHeight + _pxSelectionSub, _pxCharWidth, _pxCharHeight);
                    painter.fillRect(r, c);
                    painter.drawText(pxPosAsciiX2, pxPosY, QChar(ch));
                    pxPosAsciiX2 += _pxCharWidth;
                }
            }

            //painter.drawLine(pxOfsX, pxPosY +10 , pxOfsX + 800, pxPosY + 10);


        }


        painter.setBackgroundMode(Qt::TransparentMode);
        painter.setPen(viewport()->palette().color(QPalette::WindowText));
    }

    // _cursorPosition counts in 2, _bPosFirst counts in 1
    int hexPositionInShowData = _cursorPosition - 2 * _bPosFirst;

    // due to scrolling the cursor can go out of the currently displayed data
    if ((hexPositionInShowData >= 0) && (hexPositionInShowData < _hexDataShown.size()))
    {
        // paint cursor
        if (_readOnly)
        {
            // make the background stick out
            QColor color = viewport()->palette().dark().color();
            painter.fillRect(QRect(_pxCursorX - pxOfsX, _pxCursorY - _pxCharHeight + _pxSelectionSub, _pxCharWidth, _pxCharHeight), color);
        }
        else
        {
            if (_blink && hasFocus())
                painter.fillRect(_cursorRect, this->palette().color(QPalette::WindowText));
        }

        if (_editAreaIsAscii)
        {
            // every 2 hex there is 1 ascii
            int asciiPositionInShowData = hexPositionInShowData / 2;
            int ch = (uchar)_dataShown.at(asciiPositionInShowData);
            if (ch < ' ' || ch > '~')
                ch = '.';
            painter.drawText(_pxCursorX - pxOfsX, _pxCursorY, QChar(ch));
        }
        else
        {
            QByteArray mar = _hexDataShown.mid(hexPositionInShowData, 1);
            painter.drawText(_pxCursorX - pxOfsX, _pxCursorY, hexCaps()?mar.toUpper():mar);
        }
    }

    // emit event, if size has changed
    if (_lastEventSize != _chunks->size())
    {
        _lastEventSize = _chunks->size();
        emit currentSizeChanged(_lastEventSize);
    }
}

void QHexEdit_hexedit::resizeEvent(QResizeEvent *)
{
    if (_dynamicBytesPerLine)
    {
        int pxFixGaps = 0;
        if (_addressArea)
            pxFixGaps = addressWidth() * _pxCharWidth + _pxGapAdr;
        pxFixGaps += _pxGapAdrHex;
        if (_asciiArea)
            pxFixGaps += _pxGapHexAscii;

        // +1 because the last hex value do not have space. so it is effective one char more
        int charWidth = (viewport()->width() - pxFixGaps ) / _pxCharWidth + 1;

        // 2 hex alfa-digits 1 space 1 ascii per byte = 4; if ascii is disabled then 3
        // to prevent devision by zero use the min value 1
        setBytesPerLine(std::max(charWidth / (_asciiArea ? 4 : 3),1));
    }
    adjust();
}

bool QHexEdit_hexedit::focusNextPrevChild(bool next)
{
    if (_addressArea)
    {
        if ( (next && _editAreaIsAscii) || (!next && !_editAreaIsAscii ))
            return QWidget::focusNextPrevChild(next);
        else
            return false;
    }
    else
    {
        return QWidget::focusNextPrevChild(next);
    }
}

// ********************************************************************** Handle selections
void QHexEdit_hexedit::resetSelection()
{
    _bSelectionBegin = _bSelectionInit;
    _bSelectionEnd = _bSelectionInit;
}

void QHexEdit_hexedit::resetSelection(qint64 pos)
{
    pos = pos / 2 ;
    if (pos < 0)
        pos = 0;
    if (pos > _chunks->size())
        pos = _chunks->size();

    _bSelectionInit = pos;
    _bSelectionBegin = pos;
    _bSelectionEnd = pos;
}

void QHexEdit_hexedit::setSelection(qint64 pos)
{
    pos = pos / 2;
    if (pos < 0)
        pos = 0;
    if (pos > _chunks->size())
        pos = _chunks->size();

    if (pos >= _bSelectionInit)
    {
        _bSelectionEnd = pos;
        _bSelectionBegin = _bSelectionInit;
    }
    else
    {
        _bSelectionBegin = pos;
        _bSelectionEnd = _bSelectionInit;
    }

    emit signal_selected_data_offsets(_bSelectionBegin, _bSelectionEnd);
}

qint64 QHexEdit_hexedit::getSelectionBegin()
{
    return _bSelectionBegin;
}

qint64 QHexEdit_hexedit::getSelectionEnd()
{
    return _bSelectionEnd;
}

// ********************************************************************** Private utility functions
void QHexEdit_hexedit::init()
{
    _undoStack->clear();
    setAddressOffset(0);
    resetSelection(0);
    setCursorPosition(0);
    verticalScrollBar()->setValue(0);
    _modified = false;
}

void QHexEdit_hexedit::adjust()
{
    // recalc Graphics
    if (_addressArea)
    {
        _addrDigits = addressWidth();
        _pxPosHexX = _pxGapAdr + _addrDigits*_pxCharWidth + _pxGapAdrHex;
    }
    else
        _pxPosHexX = _pxGapAdrHex;
    _pxPosAdrX = _pxGapAdr;
    _pxPosAsciiX = _pxPosHexX + _hexCharsInLine * _pxCharWidth + _pxGapHexAscii;

    // set horizontalScrollBar()
    int pxWidth = _pxPosAsciiX;
    if (_asciiArea)
        pxWidth += _bytesPerLine*_pxCharWidth;
    horizontalScrollBar()->setRange(0, pxWidth - viewport()->width());
    horizontalScrollBar()->setPageStep(viewport()->width());

    // set verticalScrollbar()
    _rowsShown = ((viewport()->height()-14)/_pxCharHeight);
    // int lineCount = (int)(_chunks->size() / (qint64)_bytesPerLine) + 1;
    qint64 lineCount = (_chunks->size() / (qint64)_bytesPerLine) + 2; // -- Manoj

    //    verticalScrollBar()->setRange(0, lineCount - _rowsShown);
    //    verticalScrollBar()->setPageStep(_rowsShown);

    qint64 value = 0;
    if(scroll_bar_64_obj != NULL)
    {
        scroll_bar_64_obj->SetMinPos(0);
        scroll_bar_64_obj->SetMaxPos(lineCount - _rowsShown);

        value = scroll_bar_64_obj->value();

    }

    //int value = verticalScrollBar()->value();
    //_bPosFirst = (qint64)value * _bytesPerLine;
    _bPosFirst = value * _bytesPerLine;

    _bPosLast = _bPosFirst + (qint64)(_rowsShown * _bytesPerLine) - 1;
    if (_bPosLast >= _chunks->size())
        _bPosLast = _chunks->size() - 1;
    readBuffers();

    setCursorPosition(_cursorPosition);
}

void QHexEdit_hexedit::dataChangedPrivate(int)
{
    _modified = _undoStack->index() != 0;
    adjust();
    emit dataChanged();
}

void QHexEdit_hexedit::refresh()
{
    ensureVisible();
    readBuffers();
}

void QHexEdit_hexedit::readBuffers()
{
    _dataShown = _chunks->data(_bPosFirst, _bPosLast - _bPosFirst + _bytesPerLine, &_markedShown);
    // _dataShown = _chunks->data(_bPosFirst, _bPosLast - _bPosFirst + _bytesPerLine + 1, &_markedShown); // -- Manoj
    _hexDataShown = QByteArray(_dataShown.toHex());
}

QString QHexEdit_hexedit::toReadable(const QByteArray &ba)
{
    QString result;

    for (int i=0; i < ba.size(); i += 16)
    {
        QString addrStr = QString("%1").arg(_addressOffset + i, addressWidth(), 16, QChar('0'));
        QString hexStr;
        QString ascStr;
        for (int j=0; j<16; j++)
        {
            if ((i + j) < ba.size())
            {
                hexStr.append(" ").append(ba.mid(i+j, 1).toHex());
                char ch = ba[i + j];
                if ((ch < 0x20) || (ch > 0x7e))
                    ch = '.';
                ascStr.append(QChar(ch));
            }
        }
        result += addrStr + " " + QString("%1").arg(hexStr, -48) + "  " + QString("%1").arg(ascStr, -17) + "\n";
    }
    return result;
}

void QHexEdit_hexedit::updateCursor()
{
    if (_blink)
        _blink = false;
    else
        _blink = true;
    viewport()->update(_cursorRect);
}

void QHexEdit_hexedit::slot_scroll_bar_64_changed(qint64 scrol_pos)
{
    adjust();

    viewport()->update();
}


bool QHexEdit_hexedit::highlight_block_bytes_fun(qint64 cur_char,QColor &col)
{

    for(int i = 0; i < highlight_tags_list.size(); i++)
    {
        qint64 block_actual_start_in_file = highlight_tags_list.at(i).start_address;
        qint64 block_actual_end_in_file = highlight_tags_list.at(i).end_address;


        if(cur_char >= block_actual_start_in_file && cur_char <= block_actual_end_in_file)
        {
            col = QColor("#D59B3F");
            return true;
        }
    }


    for(int i = tags_list->size() - 1; i >=0 ; i--)
    {
        qint64 block_actual_start_in_file = tags_list->at(i).start_address;
        qint64 block_actual_end_in_file = tags_list->at(i).end_address;


        if(cur_char >= block_actual_start_in_file && cur_char <= block_actual_end_in_file)
        {
            col = QColor(tags_list->at(i).color_hex);
            return true;
        }
    }

    return false;

    //        uint64_t block_start_index = list_struct_indexs.at(i).start_index;
    //        uint64_t block_end_offset = list_struct_indexs.at(i).end_index;

    //        if(((block_actual_start_in_file >= current_page_start_offset) && (block_actual_start_in_file <= current_page_end_offset)) && ((block_actual_end_in_file >= current_page_start_offset) && (block_actual_end_in_file <= current_page_end_offset)))
    //        {
    //            if((cur_char >= block_start_index) && (cur_char <= block_end_offset))
    //            {
    //                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
    //                painter.setBackgroundMode(Qt::OpaqueMode);

    //            }
    //        }
    //        else if(((block_actual_start_in_file >= current_page_start_offset) && (block_actual_start_in_file <= current_page_end_offset)) && ((block_actual_end_in_file > current_page_end_offset)))
    //        {
    //            if((cur_char >= list_struct_indexs.at(i).start_index) && (cur_char <= _data.size()))
    //            {
    //                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
    //                painter.setBackgroundMode(Qt::OpaqueMode);
    //            }
    //        }
    //        else if((block_actual_end_in_file >= current_page_start_offset) && (block_actual_end_in_file <= current_page_end_offset) && ((block_actual_start_in_file < current_page_start_offset)))
    //        {
    //            if((cur_char >= 0 && cur_char) <= list_struct_indexs.at(i).end_index)
    //            {
    //                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
    //                painter.setBackgroundMode(Qt::OpaqueMode);
    //            }
    //        }
    //        else if(((block_actual_start_in_file <= current_page_start_offset) && (block_actual_end_in_file > current_page_start_offset)) && ((block_actual_start_in_file <= current_page_start_offset) && (block_actual_end_in_file > current_page_end_offset)))
    //        {
    //            if(cur_char >= 0 && cur_char <= _data.size())
    //            {
    //                painter.setBackground(QBrush(list_struct_indexs.at(i).block_highlighting_color));
    //                painter.setBackgroundMode(Qt::OpaqueMode);
    //            }
    //        }
    //    }
}

QByteArray QHexEdit_hexedit::pub_get_selected_bytes()
{
    return _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
}
