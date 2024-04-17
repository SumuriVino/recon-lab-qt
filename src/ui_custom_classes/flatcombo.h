#ifndef FLATCOMBO_H
#define FLATCOMBO_H

#include <QComboBox>

class QFlatComboBox : public QComboBox
{
typedef QComboBox Super;

Q_OBJECT

public:
QFlatComboBox(QWidget *parent = 0, int m_status = 0);
bool flat() const { return flat_; }
void setFlat(bool flat);

Qt::Alignment arrowAlignment() const { return arrowAlignment_; }
void setArrowAlignment(Qt::Alignment a);

protected:
virtual void paintEvent(QPaintEvent *e);
virtual void mousePressEvent(QMouseEvent *e);

signals:
void aboutToPullDown();

private:
bool flat_;
Qt::Alignment arrowAlignment_;


int received_status;
};

#endif // FLATCOMBO_H
