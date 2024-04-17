#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QPainter>
#include <QScrollBar>

#include "qhexedit.h"
#include <algorithm>


// ********************************************************************** Constructor, destructor

QHexEdit_textviewer::QHexEdit_textviewer(QWidget *parent) : QAbstractScrollArea(parent)
{
    _addressArea = true;
    _addressWidth = 8;
    _asciiArea = true;
    _highlighting = true;
    _readOnly = false;
    _cursorPosition = 0;
    _hexCharsInLine = 0;
    _bytesPerLine = 64;
    _editAreaIsAscii = false;
    _hexCaps = false;
    _dynamicBytesPerLine = false;


    recon_helper_standard_obj = new recon_helper_standard(this);

    mainMenu = new QMenu(this);
    QFont m_font;
    m_font.setPointSize(MACRO_QMenu_Font_Size_int);
    mainMenu->setFont(m_font);

    connect(mainMenu, SIGNAL(triggered(QAction*)), this, SLOT(slot_right_click_main_menu_triggered(QAction*)));

    QAction *action_select_all = new QAction(mainMenu);
    action_select_all = mainMenu->addAction(tr("Select All"));
    action_select_all->setFont(m_font);

    sub_menu_tags = mainMenu->addMenu(tr(QString(MACRO_Generic_Right_Click_Tags).toLocal8Bit().data()));
    sub_menu_tags->setFont(m_font);

    connect(sub_menu_tags, SIGNAL(triggered(QAction*)), this, SLOT(slot_sub_menu_tags_clicked(QAction*)));

    add_tags_module_obj = new add_tags(this);

    connect(add_tags_module_obj, SIGNAL(tag_status(bool,QString,QString)), this, SLOT(slot_update_tags_submenu(bool,QString,QString)));
    connect(add_tags_module_obj, SIGNAL(signal_tags(QString,QString)), this, SLOT(slot_add_tags(QString,QString)));

    _chunks = new Chunks(this);
    setFont_textviewer(QFont("Courier", 12));


    setAddressAreaColor_textviewer(this->palette().alternateBase().color());
    setHighlightingColor_textviewer(QColor(0xff, 0xff, 0x99, 0xff));
    setSelectionColor(this->palette().highlight().color());

    connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursor()));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust_data()));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjust_data()));

    _cursorTimer.setInterval(500);
    _cursorTimer.start();

    setAddressWidth_textviewer(8);
    setAddressArea_textview(true);
    setAsciiArea_textview(true);
    setHighlighting(true);
    setReadOnly_textview(true);

    init();

}

QHexEdit_textviewer::~QHexEdit_textviewer()
{
}


// ********************************************************************** Properties

void QHexEdit_textviewer::setAddressArea_textview(bool addressArea)
{
    _addressArea = addressArea;
    adjust_data();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

bool QHexEdit_textviewer::addressArea()
{
    return _addressArea;
}

void QHexEdit_textviewer::setAddressAreaColor_textviewer(const QColor &color)
{
    _addressAreaColor = color;
    viewport()->update();
}

QColor QHexEdit_textviewer::addressAreaColor_textviewer()
{
    return _addressAreaColor;
}

void QHexEdit_textviewer::setAddressOffset_textviewer(qint64 addressOffset)
{
    _addressOffset_textviewer = addressOffset;
    adjust_data();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

qint64 QHexEdit_textviewer::addressOffset_textviewer()
{
    return _addressOffset_textviewer;
}

void QHexEdit_textviewer::setAddressWidth_textviewer(int addressWidth)
{
    _addressWidth = addressWidth;
    adjust_data();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

int QHexEdit_textviewer::addressWidth()
{
    qint64 size = _chunks->size();
    int n = 1;
    if (size > Q_INT64_C(0x100000000)){ n += 8; size /= Q_INT64_C(0x100000000);}
    if (size > 0x10000){ n += 4; size /= 0x10000;}
    if (size > 0x100){ n += 2; size /= 0x100;}
    if (size > 0x10){ n += 1; size /= 0x10;}

    if (n > _addressWidth)
        return n;
    else
        return _addressWidth;
}

void QHexEdit_textviewer::setAsciiArea_textview(bool asciiArea)
{
    if (!asciiArea)
        _editAreaIsAscii = false;
    _asciiArea = asciiArea;
    adjust_data();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

bool QHexEdit_textviewer::asciiArea()
{
    return _asciiArea;
}

void QHexEdit_textviewer::setBytesPerLine(int count)
{
    _bytesPerLine = count;
    adjust_data();
    setCursorPosition(_cursorPosition);
    viewport()->update();
}

int QHexEdit_textviewer::bytesPerLine()
{
    return _bytesPerLine;
}

void QHexEdit_textviewer::setCursorPosition(qint64 position)
{
    // 1. delete old cursor
    _blink = false;
    viewport()->update(_cursorRect);

    // 2. Check, if cursor in range?
    if (position > (_chunks->size() * 2 - 1))
        position = _chunks->size() * 2;

    if (position < 0)
        position = 0;

    // 3. Calc new position of cursor
    _bPosCurrent = position / 2;
    _pxCursorY = ((position / 2 - _bPosFirst) / _bytesPerLine + 1) * _pxCharHeight;
    int x = (position % (2 * _bytesPerLine));
    if (_editAreaIsAscii)
    {
        _pxCursorX = x / 2 * _pxCharWidth + _pxPosAsciiX;
        _cursorPosition = position & 0xFFFFFFFFFFFFFFFE;
    } else {
        _pxCursorX = (((x / 2) * 3) + (x % 2)) * _pxCharWidth + _pxPosHexX;
        _cursorPosition = position;
    }

    _cursorRect = QRect(_pxCursorX - horizontalScrollBar()->value(), _pxCursorY - _pxCharHeight + 4, _pxCursorWidth, _pxCharHeight);

    // 4. Immediately draw new cursor
    _blink = true;
    viewport()->update(_cursorRect);
    emit currentAddressChanged(_bPosCurrent);
}

qint64 QHexEdit_textviewer::cursorPosition(QPoint pos)
{
    // Calc cursor position depending on a graphical position
    qint64 result = -1;
    int posX = pos.x() + horizontalScrollBar()->value();
    int posY = pos.y() - 3;
    //    if ((posX >= _pxPosHexX) && (posX < (_pxPosHexX + (1 + _hexCharsInLine) * _pxCharWidth)))
    if ((posX >= _pxPosHexX) && (posX <  _pxCharWidth))
    {
        _editAreaIsAscii = false;
        int x = (posX - _pxPosHexX) / _pxCharWidth;
        x = (x / 3) * 2 + x % 3;
        int y = (posY / _pxCharHeight) * 2 * _bytesPerLine;
        result = _bPosFirst * 2 + x + y;
    }
    else
        if (_asciiArea && (posX >= _pxPosAsciiX) && (posX < (_pxPosAsciiX + (1 + _bytesPerLine) * _pxCharWidth)))
        {
            _editAreaIsAscii = true;
            int x = 2 * (posX - _pxPosAsciiX) / _pxCharWidth;
            int y = (posY / _pxCharHeight) * 2 * _bytesPerLine;
            result = _bPosFirst * 2 + x + y;
        }
    return result;
}

qint64 QHexEdit_textviewer::cursorPosition()
{
    return _cursorPosition;
}

void QHexEdit_textviewer::setData_text_viewer(const QByteArray &ba)
{
    if(ba.isEmpty())
        return;

    _bData_textviewer.setData(ba);
    setData_text_viewer(_bData_textviewer);
}

void QHexEdit_textviewer::clear_text_viewer()
{
    list_struct_indexes.clear();

    QByteArray blank_arr;
    blank_arr.clear();

    _bData_textviewer.setData(blank_arr);
    setData_text_viewer(_bData_textviewer);}

void QHexEdit_textviewer::pub_set_essentials()
{

        create_tags_submenu();

}


QByteArray QHexEdit_textviewer::data_textviewer()
{
    if(_chunks->size() > 0)
        return _chunks->data(0, -1);
    else
        return QByteArray();
}

void QHexEdit_textviewer::setHighlighting(bool highlighting)
{
    _highlighting = highlighting;
    viewport()->update();
}

bool QHexEdit_textviewer::highlighting()
{
    return _highlighting;
}

void QHexEdit_textviewer::setHighlightingColor_textviewer(const QColor &color)
{
    _brushHighlighted = QBrush(color);
    _penHighlighted = QPen(viewport()->palette().color(QPalette::WindowText));
    viewport()->update();
}

QColor QHexEdit_textviewer::highlightingColor_textviewer()
{
    return _brushHighlighted.color();
}

void QHexEdit_textviewer::setSelectionColor(const QColor &color)
{
    _brushSelection = QBrush(color);
    _penSelection = QPen(Qt::black);
    viewport()->update();
}

QColor QHexEdit_textviewer::selectionColor()
{
    return _brushSelection.color();
}

bool QHexEdit_textviewer::isReadOnly()
{
    return _readOnly;
}

void QHexEdit_textviewer::setReadOnly_textview(bool readOnly)
{
    _readOnly = readOnly;
}

void QHexEdit_textviewer::setHexCaps(const bool isCaps)
{
    if (_hexCaps != isCaps)
    {
        _hexCaps = isCaps;
        viewport()->update();
    }
}

bool QHexEdit_textviewer::hexCaps()
{
    return _hexCaps;
}

void QHexEdit_textviewer::setDynamicBytesPerLine(const bool isDynamic)
{
    _dynamicBytesPerLine = isDynamic;
    resizeEvent(NULL);
}

bool QHexEdit_textviewer::dynamicBytesPerLine()
{
    return _dynamicBytesPerLine;
}

// ********************************************************************** Access to data of qhexedit
bool QHexEdit_textviewer::setData_text_viewer(QIODevice &iODevice)
{
    bool ok = _chunks->setIODevice(iODevice);
    init();
    dataChangedPrivate();
    return ok;
}


// ********************************************************************** Char handling
void QHexEdit_textviewer::insert(qint64 index, char ch)
{
    Q_UNUSED(index);
    Q_UNUSED(ch);

    refresh();
}

void QHexEdit_textviewer::remove(qint64 index, qint64 len)
{
    Q_UNUSED(index);
    Q_UNUSED(len);

    refresh();
}

void QHexEdit_textviewer::replace(qint64 index, char ch)
{
    Q_UNUSED(index);
    Q_UNUSED(ch);

    refresh();
}

// ********************************************************************** ByteArray handling
void QHexEdit_textviewer::insert(qint64 pos, const QByteArray &ba)
{
    Q_UNUSED(pos);
    Q_UNUSED(ba);


    refresh();
}

void QHexEdit_textviewer::replace(qint64 pos, qint64 len, const QByteArray &ba)
{
    Q_UNUSED(pos);
    Q_UNUSED(len);
    Q_UNUSED(ba);


    refresh();
}

// ********************************************************************** Utility functions
void QHexEdit_textviewer::ensureVisible()
{
    if (_cursorPosition < (_bPosFirst * 2))
        verticalScrollBar()->setValue(static_cast<int>(_cursorPosition / 2 / _bytesPerLine));
    if (_cursorPosition > ((_bPosFirst + (_rowsShown - 1)*_bytesPerLine) * 2))
        verticalScrollBar()->setValue(static_cast<int>(_cursorPosition / 2 / _bytesPerLine) - _rowsShown + 1);
    if (_pxCursorX < horizontalScrollBar()->value())
        horizontalScrollBar()->setValue(_pxCursorX);
    if ((_pxCursorX + _pxCharWidth) > (horizontalScrollBar()->value() + viewport()->width()))
        horizontalScrollBar()->setValue(_pxCursorX + _pxCharWidth - viewport()->width());
    viewport()->update();

}

void QHexEdit_textviewer::redo()
{
    setCursorPosition(_chunks->pos()*(_editAreaIsAscii ? 1 : 2));
    refresh();
}


void QHexEdit_textviewer::setFont_textviewer(const QFont &font)
{
    QWidget::setFont(font);
    _pxCharWidth = fontMetrics().horizontalAdvance(QLatin1Char('2'));
    _pxCharHeight = fontMetrics().height();
    _pxGapAdr = _pxCharWidth / 2;
    _pxGapAdrHex = _pxCharWidth;
    _pxGapHexAscii = 2 * _pxCharWidth;
    _pxCursorWidth = _pxCharHeight / 7;
    _pxSelectionSub = _pxCharHeight / 5;
    viewport()->update();
}


void QHexEdit_textviewer::undo()
{
    //    _undoStack->undo();
    setCursorPosition(_chunks->pos()*(_editAreaIsAscii ? 1 : 2));
    refresh();
}

// ********************************************************************** Handle events
void QHexEdit_textviewer::keyPressEvent(QKeyEvent *event)
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

            remove(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
            setCursorPosition(2 * getSelectionBegin());
            resetSelection(2 * getSelectionBegin());
        } else

            /* Paste */
            if (event->matches(QKeySequence::Paste))
            {
                QClipboard *clipboard = QApplication::clipboard();
                QByteArray ba = QByteArray().fromHex(clipboard->text().toLatin1());

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

                        remove(_bPosCurrent, getSelectionEnd() - getSelectionBegin());
                    }
                    else
                    {
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

                            remove(_bPosCurrent, getSelectionEnd() - getSelectionBegin());
                            resetSelection(2 * _bPosCurrent);
                        }
                        else
                        {
                            bool behindLastByte = false;
                            if ((_cursorPosition / 2) == _chunks->size())
                                behindLastByte = true;

                            _bPosCurrent -= 1;

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

                                            remove(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
                                            _bPosCurrent = getSelectionBegin();
                                            setCursorPosition(2 * _bPosCurrent);
                                            resetSelection(2 * _bPosCurrent);
                                        }


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
        ///===Hex data copy====///
        //        QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin()).toHex();
        //        for (qint64 idx = 32; idx < ba.size(); idx +=33)
        //            ba.insert(idx, "\n");
        //        QClipboard *clipboard = QApplication::clipboard();
        //        clipboard->setText(ba);

        ///===ASCII data copy====///
        QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
        std::string std_str(ba.constData(), ba.length());
        QString ascii_str = QString::fromStdString(std_str);
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ascii_str);

    }

    // Switch between insert/overwrite mode
    if ((event->key() == Qt::Key_Insert) && (event->modifiers() == Qt::NoModifier))
    {
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

void QHexEdit_textviewer::mouseMoveEvent(QMouseEvent * event)
{
    _blink = false;
    viewport()->update();
    qint64 actPos = cursorPosition(event->pos());
    if (actPos >= 0)
    {
        setCursorPosition(actPos);
        setSelection(actPos);
    }
}

void QHexEdit_textviewer::mousePressEvent(QMouseEvent * event)
{

    if(event->button() == Qt::RightButton)
    {
        QAbstractScrollArea::mousePressEvent(event);
        mainMenu->exec(event->globalPos());
    }
    else if(event->button() == Qt::LeftButton)
    {
        _blink = false;
        viewport()->update();
        qint64 cPos = cursorPosition(event->pos());
        if (cPos >= 0)
        {
            resetSelection(cPos);
            setCursorPosition(cPos);
        }
    }
    else
    {
        //unrecognised key
    }

}

void QHexEdit_textviewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    int pxOfsX = horizontalScrollBar()->value();

    if (event->rect() != _cursorRect)
    {
        int pxPosStartY = _pxCharHeight;


        // draw some patterns if needed
        painter.fillRect(event->rect(), viewport()->palette().color(QPalette::Base));
        if (_addressArea)
            painter.fillRect(QRect(-pxOfsX, event->rect().top(), _pxPosHexX - _pxGapAdrHex/2, height()), _addressAreaColor);
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
            QString address;
            for (int row=0, pxPosY = _pxCharHeight; row <= (_dataShown.size()/_bytesPerLine); row++, pxPosY +=_pxCharHeight)
            {
                address = QString("%1").arg(_bPosFirst + row*_bytesPerLine + _addressOffset_textviewer, _addrDigits, 16, QChar('0'));
                painter.drawText(_pxPosAdrX - pxOfsX, pxPosY, address);
            }
        }

        // paint hex and ascii area
        QPen colStandard = QPen(viewport()->palette().color(QPalette::WindowText));

        painter.setBackgroundMode(Qt::TransparentMode);

        for (int row = 0, pxPosY = pxPosStartY; row <= _rowsShown; row++, pxPosY +=_pxCharHeight)
        {
            //            QByteArray hex;
            //            int pxPosX = _pxPosHexX  - pxOfsX;

            int pxPosAsciiX2 = _pxPosAsciiX  - pxOfsX;
            qint64 bPosLine = row * _bytesPerLine;
            for (int colIdx = 0; ((bPosLine + colIdx) < _dataShown.size() && (colIdx < _bytesPerLine)); colIdx++)
            {
                QColor c = viewport()->palette().color(QPalette::Base);
                painter.setPen(colStandard);

                qint64 posBa = _bPosFirst + bPosLine + colIdx;
                if ((getSelectionBegin() <= posBa) && (getSelectionEnd() > posBa))
                {
                    c = _brushSelection.color();
                    painter.setPen(_penSelection);
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

                if(list_struct_indexes.size() > 0)
                {
                    int cur_char = (bPosLine + colIdx);
                    set_tag_color_text_viewer(cur_char,painter);
                }

                painter.setPen(Qt::black);


                // render hex value
                QRect r;
                //                if (colIdx == 0)
                //                    r.setRect(pxPosX, pxPosY - _pxCharHeight + _pxSelectionSub, 2*_pxCharWidth, _pxCharHeight);
                //                else
                //                    r.setRect(pxPosX - _pxCharWidth, pxPosY - _pxCharHeight + _pxSelectionSub, 3*_pxCharWidth, _pxCharHeight);
                //                painter.fillRect(r, c);
                //                hex = _hexDataShown.mid((bPosLine + colIdx) * 2, 2);
                //                painter.drawText(pxPosX, pxPosY, hexCaps()?hex.toUpper():hex);
                //                pxPosX += 3*_pxCharWidth;

                // render ascii value
                if(_asciiArea && _dataShown.size() > (bPosLine + colIdx))
                {
                    int ch = (uchar)_dataShown.at(bPosLine + colIdx);

                    if (ch < 0x20 || ch < 0x7e)
                        ch = '.';

                    r.setRect(pxPosAsciiX2, pxPosY - _pxCharHeight + _pxSelectionSub, _pxCharWidth, _pxCharHeight);
                    painter.fillRect(r, c);
                    painter.drawText(pxPosAsciiX2, pxPosY, QChar(ch));
                    pxPosAsciiX2 += _pxCharWidth;

                }

                painter.setBackgroundMode(Qt::TransparentMode);
                painter.setPen(viewport()->palette().color(QPalette::WindowText));

            }
        }
        painter.setBackgroundMode(Qt::TransparentMode);
        painter.setPen(viewport()->palette().color(QPalette::WindowText));
    }

    // paint cursor
    // _cursorPosition counts in 2, _bPosFirst counts in 1
    int hexPositionInShowData = _cursorPosition - 2 * _bPosFirst;

    // due to scrolling the cursor can go out of the currently displayed data
    //    if ((hexPositionInShowData >= 0) && (hexPositionInShowData < _hexDataShown.size()))
    if (hexPositionInShowData >= 0)
    {
        painter.fillRect(QRect(_pxCursorX - pxOfsX, _pxCursorY - _pxCharHeight + _pxSelectionSub, _pxCharWidth, _pxCharHeight), viewport()->palette().dark().color());
        if (_editAreaIsAscii)
        {
            // every 2 hex there is 1 ascii
            int asciiPositionInShowData = hexPositionInShowData / 2;
            if(_dataShown.size() > asciiPositionInShowData)
            {
                int ch = (uchar)_dataShown.at(asciiPositionInShowData);
                if (ch < ' ')
                    ch = '.';

                painter.drawText(_pxCursorX - pxOfsX, _pxCursorY, QChar(ch));

            }
        }

    }

}

void QHexEdit_textviewer::resizeEvent(QResizeEvent *)
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
    adjust_data();
}

bool QHexEdit_textviewer::focusNextPrevChild(bool next)
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
void QHexEdit_textviewer::resetSelection()
{
    _bSelectionBegin = _bSelectionInit;
    _bSelectionEnd = _bSelectionInit;
}

void QHexEdit_textviewer::resetSelection(qint64 pos)
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

void QHexEdit_textviewer::setSelection(qint64 pos)
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
}

int QHexEdit_textviewer::getSelectionBegin()
{
    return static_cast<int>(_bSelectionBegin);
}

int QHexEdit_textviewer::getSelectionEnd()
{
    return static_cast<int>(_bSelectionEnd);
}

// ********************************************************************** Private utility functions
void QHexEdit_textviewer::init()
{
    //    _undoStack->clear();
    setAddressOffset_textviewer(0);
    resetSelection(0);
    setCursorPosition(0);
    verticalScrollBar()->setValue(0);
}

void QHexEdit_textviewer::adjust_data()
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
    _pxPosAsciiX =  _pxGapHexAscii + _hexCharsInLine + _pxPosHexX;

    // set horizontalScrollBar()
    int pxWidth = _pxPosAsciiX;
    if (_asciiArea)
        pxWidth += _bytesPerLine*_pxCharWidth;
    horizontalScrollBar()->setRange(0, pxWidth - viewport()->width());
    horizontalScrollBar()->setPageStep(viewport()->width());

    // set verticalScrollbar()
    _rowsShown = ((viewport()->height()-4)/_pxCharHeight);
    int lineCount = (int)(_chunks->size() / (qint64)_bytesPerLine) + 1;
    verticalScrollBar()->setRange(0, lineCount - _rowsShown);
    verticalScrollBar()->setPageStep(_rowsShown);

    int value = verticalScrollBar()->value();
    _bPosFirst = (qint64)value * _bytesPerLine;
    _bPosLast = _bPosFirst + (qint64)(_rowsShown * _bytesPerLine) - 1;
    if (_bPosLast >= _chunks->size())
        _bPosLast = _chunks->size() - 1;
    readBuffers();
    setCursorPosition(_cursorPosition);
}

void QHexEdit_textviewer::dataChangedPrivate(int)
{
    adjust_data();
    emit dataChanged();
}

void QHexEdit_textviewer::refresh()
{
    ensureVisible();
    readBuffers();
}

void QHexEdit_textviewer::readBuffers()
{
    qint64 max_size = _bPosLast - _bPosFirst;

    if(_chunks->size()  > max_size)
        _dataShown = _chunks->data(_bPosFirst, max_size, &_markedShown);
}

void QHexEdit_textviewer::updateCursor()
{
    if (_blink)
        _blink = false;
    else
        _blink = true;
    viewport()->update(_cursorRect);
}

void QHexEdit_textviewer::slot_right_click_main_menu_triggered(QAction* current_clicked_action)
{
    if(current_clicked_action->text() == QString("Select All"))
    {
        resetSelection(0);
        setSelection(2 * _chunks->size() + 1);

        refresh();
    }

}

void QHexEdit_textviewer::create_tags_submenu()
{
    recon_static_functions::app_debug(" -Starts",Q_FUNC_INFO);

    if(sub_menu_tags == NULL)
        return;

    sub_menu_tags->clear();

    ///add new tag
    act_create_tag = new QAction(QString(MACRO_Generic_Right_Click_Create_Tag), this);
    QIcon icon_create("../icons/tag_icons/add.png");
    act_create_tag->setIcon(icon_create);
    sub_menu_tags->addAction(act_create_tag);

    disconnect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));
    connect(act_create_tag, SIGNAL(triggered()), this, SLOT(slot_act_create_tags()));


    ///remove tag
    //    act_remove_tag = new QAction(QString(MACRO_Generic_Right_Click_Remove_Tag), this);
    //    QIcon icon_remove("../icons/tag_icons/remove.png");
    //    act_remove_tag->setIcon(icon_remove);
    //    sub_menu_tags->addAction(act_remove_tag);

    //    disconnect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags()));
    //    connect(act_remove_tag, SIGNAL(triggered()), this, SLOT(slot_act_remove_tags()));

    QAction *separator_main = new QAction(this);
    separator_main->setSeparator(true);
    sub_menu_tags->addAction(separator_main);

    add_tags_module_obj->pub_set_essentials();

    QString command_str = "SELECT tag_name,selected_colour FROM tags";

    QStringList tags_list = recon_helper_standard_obj->get_tags_and_colours_list_from_db(command_str , add_tags_module_obj->tags_db_path, Q_FUNC_INFO);

    for(int i = 0; i < tags_list.size(); i++)
    {
        QString temp_tag_str = tags_list.at(i);

        if(temp_tag_str.contains("::"))
        {
            if(i == MACRO_DEFAULT_TAGS_LIST_SIZE_int)
            {
                QAction *separator_default_tags = new QAction(this);
                separator_default_tags->setSeparator(true);
                sub_menu_tags->addAction(separator_default_tags);
            }

            QStringList tags_splitted_list = temp_tag_str.split("::", Qt::SkipEmptyParts);

            QString icon_path = "";
            QString tag_str  = tags_splitted_list.at(0);
            QString tag_colour = tags_splitted_list.at(1);

            icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);

            QIcon icon(icon_path);

            QAction *act_already_saved_tags = new QAction(QObject::tr(tag_str.toLocal8Bit().data()), this);
            act_already_saved_tags->setIcon(icon);
            sub_menu_tags->addAction(act_already_saved_tags);
        }
    }

    recon_static_functions::app_debug(" -Ends",Q_FUNC_INFO);
}

void QHexEdit_textviewer::slot_act_create_tags()
{
    add_tags_module_obj->show();
}

void QHexEdit_textviewer::slot_update_tags_submenu(bool status, QString new_tag, QString tag_colour)
{
    recon_static_functions::app_debug(" -Starts " , Q_FUNC_INFO);

    if(new_tag == MACRO_Generic_Right_Click_Create_Tag)
        return;

    ///update new tag in actions menu
    if(status)
    {
        QString icon_path = recon_static_functions::get_tag_icon_path_from_colour_name(tag_colour, Q_FUNC_INFO);
        QIcon icon(icon_path);

        QAction *act_already_saved_tags = new QAction(QObject::tr(new_tag.toLocal8Bit().data()), this);
        act_already_saved_tags->setIcon(icon);
        sub_menu_tags->addAction(act_already_saved_tags);

    }

    recon_static_functions::app_debug(" -Ends " , Q_FUNC_INFO);
}

void QHexEdit_textviewer::slot_add_tags(QString tag_data , QString colour)
{
    update_tags_value(tag_data,colour);
}

void QHexEdit_textviewer::update_tags_value(QString tag_data,QString colour)
{
    QString icon_path =  recon_static_functions::get_tags_icon_path(colour,tag_data, Q_FUNC_INFO);
}

void QHexEdit_textviewer::slot_sub_menu_tags_clicked(QAction* current_clicked_action)
{
    action_submenu_tag_triggered(current_clicked_action->text());
}

void QHexEdit_textviewer::action_submenu_tag_triggered(QString tag_name)
{
    recon_static_functions::app_debug(" Starts " , Q_FUNC_INFO);

    if(tag_name == QString(MACRO_Generic_Right_Click_Remove_Tag) || tag_name == QString(MACRO_Generic_Right_Click_Create_Tag))
        return;

    QString command_str = "SELECT color_hex FROM tags Where tag_name = ?";
    QStringList value_list;
    value_list << tag_name;
    color_hex_tag = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);

    command_str = "SELECT selected_colour FROM tags Where tag_name = ?";
    QString selected_color = recon_helper_standard_obj->get_string_from_db_with_addbindvalues_by_dbpath(command_str,value_list ,0,add_tags_module_obj->tags_db_path,Q_FUNC_INFO);


    start_index_tag = getSelectionBegin();
    end_index_tag   = getSelectionEnd();

    if(end_index_tag > start_index_tag)
    {
        QByteArray ba = _chunks->data(getSelectionBegin(), getSelectionEnd() - getSelectionBegin());
        emit signal_add_tag_to_tag_search(start_index_tag , end_index_tag, tag_name, selected_color, color_hex_tag,ba);
    }

    struct_text_viewer_start_and_end_index struct_obj_1;
    struct_obj_1.start_index = start_index_tag;
    struct_obj_1.end_index   = end_index_tag;
    struct_obj_1.block_highlighting_color = QColor(selected_color);

    list_struct_indexes.append(struct_obj_1);


    recon_static_functions::app_debug(" Ends " , Q_FUNC_INFO);
}


void QHexEdit_textviewer::set_tag_color_text_viewer(int cur_char,QPainter &painter)
{
    for(int i = 0; i < list_struct_indexes.size(); i++)
    {
        uint64_t block_start_index = list_struct_indexes.at(i).start_index;
        uint64_t block_end_offset = list_struct_indexes.at(i).end_index;

        if(cur_char >= block_start_index && cur_char <= block_end_offset)
        {
            painter.setBackground(QBrush(list_struct_indexes.at(i).block_highlighting_color));
            painter.setBackgroundMode(Qt::OpaqueMode);
        }
    }
}
