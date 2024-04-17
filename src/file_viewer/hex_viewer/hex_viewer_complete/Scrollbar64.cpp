#include "Scrollbar64.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

namespace
{
    const double markHeightGreater = 3;
}

Scrollbar64::Scrollbar64(QWidget *parent)
    : QWidget(parent)
{
}

Scrollbar64::~Scrollbar64()
{

}

void Scrollbar64::SetOrientation(Qt::Orientation nOrientation)
{
    if (m_nOrientation != nOrientation)
        m_nOrientation = nOrientation;
}

qint64 Scrollbar64::GetMinPos() const
{
    return m_nMinPos;
}

void Scrollbar64::SetMinPos(qint64 nLower)
{
    if (m_nMinPos != nLower)
        m_nMinPos = nLower;
}

qint64 Scrollbar64::GetMaxPos() const
{
    return m_nMaxPos;
}

void Scrollbar64::SetMaxPos(qint64 nPos)
{
    if (m_nMaxPos != nPos)
        m_nMaxPos = nPos;
}

qint64 Scrollbar64::value() const
{
    return m_bInvertedAppearance ? (m_nMaxPos - m_nCurPos) : m_nCurPos;
}

void Scrollbar64::SetCurPos(qint64 nPos)
{
    if (m_nCurPos != nPos)
    {
        if (nPos < m_nMinPos)
            m_nCurPos = m_nMinPos;
        else if (nPos > m_nMaxPos)
            m_nCurPos = m_nMaxPos;
        else
            m_nCurPos = nPos;

        emit ScrollbarChanged(value());
    }
}

qint64 Scrollbar64::GetCurPos()
{
    return m_nCurPos;
}

void Scrollbar64::setInvertedAppearance(bool _inverted)
{
    if (m_bInvertedAppearance != _inverted) {
        m_bInvertedAppearance = _inverted;
    }
}

void Scrollbar64::paintEvent(QPaintEvent* _event)
{
    Q_UNUSED(_event)

    QPainter painter(this);

    painter.fillRect(rect(), palette().base());

    if ( m_dblMinValue != m_dblMaxValue) // (!isnan(m_dblMinValue) && !isnan(m_dblMaxValue) && m_dblMinValue != m_dblMaxValue)
    {
        if (m_nOrientation == Qt::Vertical)
        {
            const double textHeight = fontMetrics().lineSpacing();
            const double markHeight = textHeight * markHeightGreater;
            const qint64 marksCount = height() / markHeight;

            painter.setPen(palette().text().color());
            QRect markRect(0, 0, width(), markHeight);
            for (qint64 markIndex = 0; markIndex <= marksCount; ++markIndex)
            {
                const double markValue = interpolatePixels(markRect.center().y());
                painter.drawText(markRect, Qt::AlignCenter, QString::number(markValue));
                markRect.moveTop(markRect.top() + markHeight);
            }
        }
        else
        {
            const double textWidth = fontMetrics().horizontalAdvance("0000");

            const double markWidth = textWidth * markHeightGreater;
            const qint64 marksCount = width() / markWidth;

            painter.setPen(palette().text().color());
            QRect markRect(0, 0, markWidth, height());
            for (qint64 markIndex = 0; markIndex <= marksCount; ++markIndex)
            {
                const double markValue = interpolatePixels(markRect.center().x());
                painter.drawText(markRect, Qt::AlignCenter, QString::number(markValue));
                markRect.moveLeft(markRect.left() + markWidth);
            }
        }
    }

    painter.setBrush(palette().dark());
    painter.setOpacity(0.5);
    painter.drawRect(handleRect());
}

void Scrollbar64::mousePressEvent(QMouseEvent* _event)
{
    m_bMousePressed = true;
    correctValue(_event->pos());
    update();
    _event->accept();
}

void Scrollbar64::mouseMoveEvent(QMouseEvent* _event)
{
    if (m_bMousePressed)
    {
        correctValue(_event->pos());
        update();
    }
    _event->accept();

    qint64 nPos = value();
    if( m_nCurPos != nPos )
    {
        m_nCurPos = nPos;
        emit ScrollbarChanged(value());
    }
}

void Scrollbar64::mouseReleaseEvent(QMouseEvent* _event)
{
    m_bMousePressed = false;
    _event->accept();
}

void Scrollbar64::leaveEvent(QEvent* _event)
{
    m_bMousePressed = false;

    QWidget::leaveEvent(_event);
}

void Scrollbar64::correctValue(const QPoint& _mousePos)
{
    const qint64 newValue = interpolateValue(m_nOrientation == Qt::Vertical ? _mousePos.y() : _mousePos.x());
    SetCurPos(newValue);
}

QSize Scrollbar64::handleSize() const
{
    const qint64 handleSideSize = fontMetrics().lineSpacing() * markHeightGreater;
    return m_nOrientation == Qt::Vertical ? QSize(width(), handleSideSize) : QSize(handleSideSize, height());
}

QRect Scrollbar64::handleRect() const
{
    if (m_nMinPos == m_nMaxPos)
    {
        return rect();
    }

    QPoint topLeft;
    if (m_nOrientation == Qt::Vertical)
    {
        topLeft = QPoint(0, interpolateScroller(m_nCurPos));
    }
    else
    {
        topLeft = QPoint(interpolateScroller(m_nCurPos), 0);
    }

    return QRect(topLeft, handleSize());
}

double Scrollbar64::interpolateValue(qint64 _value) const
{
    if (m_nOrientation == Qt::Vertical)
    {
        return m_nMinPos + double(_value - 0.) / double(height() - 0.) * double(m_nMaxPos - m_nMinPos);
    }
    else
    {
        return m_nMinPos + double(_value - 0.) / double(width() - 0.) * double(m_nMaxPos - m_nMinPos);
    }
}

double Scrollbar64::interpolatePixels(qint64 _value) const
{
    if (m_nOrientation == Qt::Vertical)
    {
        return m_dblMinValue + double(_value - 0.) / double(height() - 0.) * (m_dblMaxValue - m_dblMinValue);
    }
    else
    {
        return m_dblMinValue + double(_value - 0.) / double(width() - 0.) * (m_dblMaxValue - m_dblMinValue);
    }
}

double Scrollbar64::interpolateScroller(qint64 _value) const
{
    if (m_nOrientation == Qt::Vertical)
    {
        return 0. + double(_value - m_nMinPos) / double(m_nMaxPos - m_nMinPos) * double(height() - handleSize().height() - 0.);
    }
    else
    {
        return 0. + double(_value - m_nMinPos) / double(m_nMaxPos - m_nMinPos) * double(width() - handleSize().width() - 0.);
    }
}
