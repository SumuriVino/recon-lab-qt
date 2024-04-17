#include "qSlice.h"

#include <QGridLayout>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

QSlice::QSlice(QWidget *parent)
    : QWidget(parent)
{
    _painter = new QPainter(this);

    _valueMin = 0.;
    _valueMax = 100.;
    this->setCurrentMin(0);
    this->setCurrentMax(100);
    _cursorSize = 16.;
    _paddingSize = 4.;
    _mouseX	= 0.;
    _moved = 0;
    _onMin = false;
    _onMax = false;

    this->setMouseTracking(true);
}

QSlice::~QSlice()
{

    if(_painter != NULL)
        delete _painter;
}

QSize QSlice::sizeHint() const
{
    return QSize(480, 10);
}

int QSlice::currentMin()
{
    return qRound(_currentMin + _valueMin);
}

int QSlice::currentMax()
{
    return qRound(_currentMax + _valueMin);
}

int QSlice::valueMin()
{
    return (int)_valueMax;
}

int QSlice::valueMax()
{
    return (int)_valueMax;
}

void QSlice::setCurrentMin(int currentMin)
{
    double newCurrentMin = (double)currentMin - _valueMin;

    if (newCurrentMin >= 0)
        _currentMin = newCurrentMin;

    if (newCurrentMin > _currentMax)
        _currentMax = newCurrentMin;

    this->update();
}

void QSlice::setCurrentMax(int currentMax)
{
    double newCurrentMax = (double)currentMax - _valueMin;

    if (newCurrentMax  <= _valueMax)
        _currentMax = newCurrentMax ;

    if (newCurrentMax  < _currentMin)
        _currentMin = newCurrentMax ;

    this->update();
}

void QSlice::setValueMin(int valueMin)
{
    if (valueMin < _valueMax)
        _valueMin = valueMin;

    if (_currentMin < _valueMin)
        _currentMin = _valueMin;

    this->update();
}

void QSlice::setValueMax(int valueMax)
{
    if (valueMax > _valueMin)
        _valueMax = valueMax;

    if (_currentMax > _valueMax)
        _currentMax = _valueMax;

    this->update();
}

void QSlice::paintEvent(QPaintEvent */*event*/)
{
    _paddingSize = this->height() / 10;
    _cursorSize = this->height() / 2 - _paddingSize;

    _painter->begin(this);
    _painter->setRenderHint(QPainter::TextAntialiasing);
    _painter->setBrush(Qt::SolidPattern);

//        _painter->setPen(Qt::SolidLine);
    _painter->setPen(QColor("#A8A8A8"));


    QColor color = this->palette().color(QPalette::Window);

    QColor colorBG = color.darker(110);
    QColor colorFG = color.darker(95);

    // init variables
    QRectF rectangle;
    double minX = _currentMin / (_valueMax - _valueMin) *
            ((this->width() - 1)  - 2 * _cursorSize) + _cursorSize;
    double maxX = _currentMax / (_valueMax - _valueMin) *
            ((this->width() - 1) - 2 * _cursorSize) + _cursorSize;
    _painter->setBrush(QBrush(colorBG));
    rectangle.setCoords(
                _cursorSize, 0,
                this->width() - 1 - _cursorSize, this->height() - 1);
    _painter->drawRect(rectangle);
    _painter->setBrush(QBrush(colorFG));
    rectangle.setCoords(minX, 0, maxX, this->height() - 1);
    _painter->drawRect(rectangle);

    if (_moved != 2)
    {
        this->drawCursor(maxX, 2);
        this->drawCursor(minX, 1);
    }
    else
    {
        this->drawCursor(minX, 1);
        this->drawCursor(maxX, 2);
    }

    _painter->end();
}

void QSlice::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        _mouseX = (float)(event->x());
    _moved = 0;
    this->update();
}

void QSlice::mouseReleaseEvent(QMouseEvent *event)
{
    _moved = 0;
    this->mouseMoveEvent(event);
    this->update();
    this->printVals();

}

void QSlice::mouseMoveEvent(QMouseEvent *event)
{
    if (_onMax || _onMin)
        setCursor(Qt::SizeHorCursor);
    else
        setCursor(Qt::ArrowCursor);

    float mouseX =	(float)(event->x());
    float minX = _currentMin / (_valueMax - _valueMin) *
            ((this->width() - 1)  - 2 * _cursorSize) + _cursorSize;
    float maxX = _currentMax / (_valueMax - _valueMin) *
            ((this->width() - 1) - 2 * _cursorSize) + _cursorSize;

    _onMin = ((mouseX > (minX - _cursorSize * 1.1)) &&
              (mouseX < (minX + _cursorSize * 1.1)))
            && !_onMax
            || (_moved == 1);
    _onMax = ((mouseX > (maxX - _cursorSize * 1.1)) &&
              (mouseX < (maxX + _cursorSize * 1.1)))
            && !_onMin
            || (_moved == 2);

    if (event->buttons() & Qt::LeftButton)
    {
        if ((_moved != 2) && _onMin)
        {
            minX = mouseX * width() /
                    ((this->width() - 1)  - 2 * _cursorSize) - _cursorSize;
            if (minX < 0)	minX = 0;
            if (minX > width()) minX = width();
            _currentMin = (minX * (_valueMax - _valueMin) / (width() - 1));
            if (_currentMax < _currentMin)
                _currentMax = _currentMin;
            _moved = 1;
            emit minChanged(this->currentMin());
        }
        else if ((_moved != 1) && _onMax)
        {
            maxX = mouseX * width() /
                    ((this->width() - 1)  - 2 * _cursorSize) - _cursorSize;
            if (maxX < 0)	maxX = 0;
            if (maxX > width()) maxX = width();
            _currentMax = (maxX * (_valueMax - _valueMin) / (width() - 1));
            if (_currentMin > _currentMax)
                _currentMin = _currentMax;
            _moved = 2;
            emit maxChanged(this->currentMax());
        }
        this->update();

        // this->printVals();
    }
    else
        _moved = 0;

    _mouseX = mouseX;

    if(_onMin)
        this->setToolTip(tootip_start);
    else if(_onMax)
        this->setToolTip(tooltip_end);
}

void QSlice::drawCursor(double pos, int id)
{
    if ((id != 1) && (id != 2))
        return;

    //    QColor color = this->palette().color(QPalette::Window).dark(135);

    //    QColor color("#262626");
    QColor color("#A8A8A8");

    QRectF rectangle;

    _painter->setBrush(Qt::SolidPattern);
    _painter->setPen(Qt::NoPen);

    _painter->setBrush(QBrush(color));
    rectangle.setCoords(
                pos - _cursorSize,
                _paddingSize,
                pos + _cursorSize,
                _cursorSize / 2 + _paddingSize);
    _painter->drawRect(rectangle);

    rectangle.setCoords(
                pos - _cursorSize,
                this->height() - _paddingSize - 1,
                pos + _cursorSize,
                this->height() - _cursorSize / 2 - _paddingSize - 1);
    _painter->drawRect(rectangle);

    rectangle.setCoords(
                pos - _cursorSize,
                _cursorSize / 2 + _paddingSize,
                pos - _cursorSize / 2,
                this->height() - _cursorSize / 2 - _paddingSize - 1);
    _painter->drawRect(rectangle);

    rectangle.setCoords(
                pos + _cursorSize,
                _cursorSize / 2 + _paddingSize,
                pos + _cursorSize / 2,
                this->height() - _cursorSize / 2 - _paddingSize - 1);
    _painter->drawRect(rectangle);

    //	_painter->setBrush(Qt::NoBrush);
    //    _painter->setPen(Qt::SolidLine);

    rectangle.setCoords(
                pos - _cursorSize, _paddingSize,
                pos + _cursorSize, this->height() - _paddingSize - 1);
    //    _painter->drawRect(rectangle);

    rectangle.setCoords(
                pos - _cursorSize / 2,
                _cursorSize / 2 + _paddingSize,
                pos + _cursorSize / 2,
                this->height() - _cursorSize / 2 - _paddingSize - 1);
    _painter->drawRect(rectangle);
}

void QSlice::printVals()
{
    //			 << "-" << qRound(_currentMax + _valueMin) <<"] \t"
    //			 << "Total [" << qRound(_valueMin)
    //			 << "-" << qRound(_valueMax) << "]";

    double minimum_value = qRound(_currentMin + _valueMin);
    double maximum_value = qRound(_currentMax + _valueMin);

    emit signal_max_and_min_value(minimum_value,maximum_value);
}

void QSlice::pub_set_slider_start_and_end_tooptip(QString start_val, QString end_val)
{
    tootip_start = start_val;
    tooltip_end = end_val;

    this->update();
}

void QSlice::pub_update_slider_on_show_all()
{
    this->setCurrentMin(0);
    this->setCurrentMax(100);
}

void QSlice::pub_set_first_slider_position_moving_left_side()
{
    if(_currentMin > 0)
    {
        _currentMin = _currentMin - (MACRO_Slider_Movement_By_PushButton_int);
    }

    this->setCurrentMin(_currentMin);
    this->printVals();
}

void QSlice::pub_set_first_slider_position_moving_right_side()
{
    if(_currentMin >= 100)
        return;

    if(_currentMin >= 0)
    {
        _currentMin = _currentMin + (MACRO_Slider_Movement_By_PushButton_int);

    }

    this->setCurrentMin(_currentMin);
    this->printVals();
}

void QSlice::pub_set_second_slider_position_moving_right_side()
{
    if(_currentMax < 100)
    {
        _currentMax = _currentMax + (MACRO_Slider_Movement_By_PushButton_int);
    }

    this->setCurrentMax(_currentMax);
    this->printVals();
}

void QSlice::pub_set_second_slider_position_moving_left_side()
{
    if(_currentMax <= 0)
        return;

    if(_currentMax <= 100)
    {
        _currentMax = _currentMax - (MACRO_Slider_Movement_By_PushButton_int);

    }

    this->setCurrentMax(_currentMax);
    this->printVals();
}
