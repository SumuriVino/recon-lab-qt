#ifndef R_DOCKWIDGET_H
#define R_DOCKWIDGET_H

#include <QDockWidget>
#include <QtCore>

class r_dockwidget : public QDockWidget
{
public:
    r_dockwidget(QWidget *parent = 0);
    ~r_dockwidget();


private:
    void closeEvent(QCloseEvent *eve);
};

#endif //
