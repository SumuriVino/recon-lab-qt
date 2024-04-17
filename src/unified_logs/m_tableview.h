#ifndef M_TABLEVIEW_H
#define M_TABLEVIEW_H

#include <QtCore>
#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QMenu>
#include <QKeyEvent>
#include <QAbstractScrollArea>

class m_tableview : public QTableView
{
    Q_OBJECT
public:
    m_tableview();

signals:
   void signal_arrow_key_pressed();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    QMenu *mainMenu;
    qint64 total_row_count = 0;
    int get_previous_visible_row(int row_index);
    int get_next_visible_row(int row_index);
};

#endif // M_TABLEVIEW_H
