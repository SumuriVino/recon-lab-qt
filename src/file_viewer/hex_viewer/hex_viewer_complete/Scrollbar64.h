#ifndef SSCROLLBAR64_H
#define SSCROLLBAR64_H

#include <QWidget>

class Scrollbar64 : public QWidget
{
    Q_OBJECT

public:
    Scrollbar64(QWidget *parent = nullptr);
    ~Scrollbar64();

    void correctValue(const QPoint& _mousePos);

signals:
    void ScrollbarChanged(qint64 _value);

protected:
    void paintEvent(QPaintEvent* _event);
    void mousePressEvent(QMouseEvent* _event);
    void mouseMoveEvent(QMouseEvent* _event);
    void mouseReleaseEvent(QMouseEvent* _event);
    void leaveEvent(QEvent* _event);

public:
    void        SetOrientation(Qt::Orientation _orientation);
    qint64      GetMinPos() const;
    void        SetMinPos(qint64 nPos);
    qint64      GetMaxPos() const;
    void        SetMaxPos(qint64 nPos);

    qint64      value() const;
    void        setInvertedAppearance(bool _inverted);

    qint64      GetCurPos();
    void        SetCurPos(qint64 nPos);

private:

    Qt::Orientation m_nOrientation = Qt::Vertical;
    qint64 m_nMaxPos = 0;
    qint64 m_nMinPos = 100;
    qint64 m_nCurPos = 50;

//    double m_dblMinValue = std::numeric_limits<double>::quiet_NaN();
//    double m_dblMaxValue = std::numeric_limits<double>::quiet_NaN();
    double m_dblMinValue = 0;
    double m_dblMaxValue = 0;
    bool m_bMousePressed = false;
    bool m_bInvertedAppearance = false;

    QSize handleSize() const;
    QRect handleRect() const;
    double interpolateValue(qint64 _value) const;
    double interpolatePixels(qint64 _value) const;
    double interpolateScroller(qint64 _value) const;
};

#endif // SSCROLLBAR64_H
