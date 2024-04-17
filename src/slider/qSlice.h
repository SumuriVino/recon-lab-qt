#ifndef __QSLICE_H
#define __QSLICE_H

#include <QWidget>
#include "header_identical_2.h"
#include <QtCore>

class QMouseEvent ;
class QPainter;

class QSlice : public QWidget
{
    Q_OBJECT

    private:
        double		_currentMin;
        double		_currentMax;
        double		_valueMin;
        double		_valueMax;
        double		_cursorSize;
        double		_paddingSize;
        double 		_mouseX;
        int 		_moved;
        bool 		_onMin;
        bool 		_onMax;
        QPainter	*_painter;

        QString tootip_start;
        QString tooltip_end;

    public:
        QSlice(QWidget *parent = NULL);
        ~QSlice();

        QSize sizeHint() const;

        //getters
        int currentMin();
        int currentMax();
        int valueMin();
        int valueMax();

        //setters
        void setValueMin(int valueMin);
        void setValueMax(int valueMax);

        void pub_set_slider_start_and_end_tooptip(QString start_val, QString end_val);

        void setCurrentMin(int currentMin);
        void setCurrentMax(int currentMax);
        void pub_update_slider_on_show_all();

        void pub_set_first_slider_position_moving_left_side();
        void pub_set_first_slider_position_moving_right_side();

        void pub_set_second_slider_position_moving_left_side();
        void pub_set_second_slider_position_moving_right_side();

public slots:

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

        void drawCursor(double pos, int id);// id = 1 -> min
                                            // id = 2 -> max

    protected slots:
        void printVals();

    signals:
        void minChanged(int);
        void maxChanged(int);
        void signal_max_and_min_value(double,double);
};

#endif
