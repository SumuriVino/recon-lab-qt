#include "flatcombo.h"

#include <QStylePainter>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPalette>

QFlatComboBox::QFlatComboBox(QWidget *parent, int m_status)
    : Super(parent)
    , arrowAlignment_(Qt::AlignRight)
    , flat_(true)
{
    //    setFlat(true);
    //    setAutoFillBackground(true);
    //    QPalette plt(palette());
    //    plt.setColor(QPalette::Background, Qt::gray);
    //    setPalette(plt);

    //   setFlat(true);
    setAutoFillBackground(true);
    //       QPalette plt(palette());
    //       QColor col(84, 84, 84);

    //      // plt.setColor(QPalette::Background, Qt::gray);
    //       plt.setColor(QPalette::Background, col);
    //       plt.setColor(QPalette::Text, Qt::white);

    //  setPalette(plt);

    //       "QTreeWidget::item:has-children"
    //       "{ "
    //       "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,    stop: 0.0 #636363, stop: 0.25 #535353, stop:0.50 #434343, stop:1.0 #333333);"
    //       "color:white;"
    //       "} "


    received_status = m_status;

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::black);
    QLinearGradient gradient(this->rect().topLeft(),this->rect().bottomLeft());

    gradient.setColorAt(0,   QColor("#838383"));
    gradient.setColorAt(0.40, QColor("#737373"));
    //  gradient.setColorAt(0.31, QColor("#636363"));
    gradient.setColorAt(1.0, QColor("#333333"));


    gradient.setColorAt(0,   QColor("#CCCCCC"));
    gradient.setColorAt(0.40, QColor("#bbbbbb"));
    //  gradient.setColorAt(0.31, QColor("#636363"));
    gradient.setColorAt(1.0, QColor("#999999"));


    gradient.setColorAt(0,   QColor("#C8C8C8"));
    gradient.setColorAt(0.40, QColor("#bebebe"));
    gradient.setColorAt(1.0, QColor("#b4b4b4"));


    palette.setBrush(QPalette::Window, gradient);
    setPalette(palette);


}

void QFlatComboBox::paintEvent(QPaintEvent *e)
{
    if (flat()) {
        QStylePainter painter(this);
        painter.setPen(palette().color(QPalette::Text));
        QStyleOptionComboBox opt;
        initStyleOption(&opt);
        QString displayText(opt.currentText);
        opt.currentText = "";
        painter.drawItemText(rect(), Qt::AlignCenter, palette(), true, displayText);



        QPen mpen;
        mpen.setColor(QColor("#777777"));

        painter.setPen(mpen);

        if(received_status == 0)
        {
            const QRect rcOld(opt.rect);
            opt.rect = QStyle::alignedRect(Qt::LeftToRight, arrowAlignment(), QSize(16, rcOld.height() - 5), rcOld);
            painter.drawPrimitive(QStyle::PE_IndicatorSpinUp, opt);

            const QRect rcOld1(opt.rect);
            opt.rect = QStyle::alignedRect(Qt::LeftToRight, arrowAlignment(), QSize(16, rcOld1.height() + 15), rcOld1);
            painter.drawPrimitive(QStyle::PE_IndicatorSpinDown, opt);
            // painter.drawPrimitive(QStyle::PE_IndicatorSpinUp, opt);

            opt.rect = rcOld;
        }

        painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
    } else {
        Super::paintEvent(e);
    }
}

void QFlatComboBox::mousePressEvent(QMouseEvent *e)
{
    if (!isEditable() || !lineEdit()->rect().contains(e->pos())) {
        emit aboutToPullDown();
    }
    Super::mousePressEvent(e);
}

void QFlatComboBox::setFlat(bool flat)
{
    flat_ = flat;
}
void QFlatComboBox::setArrowAlignment(Qt::Alignment a)
{
    arrowAlignment_ = a;
}

