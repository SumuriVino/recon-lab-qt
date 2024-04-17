#ifndef R_DETACH_DETAILED_INFORMATION_H
#define R_DETACH_DETAILED_INFORMATION_H

#include <QDialog>
#include <QtCore>
#include <QHBoxLayout>
#include <QSplitter>

namespace Ui {
class detach_detailed_information;
}

class detach_detailed_information : public QWidget
{
    Q_OBJECT

public:
    explicit detach_detailed_information(QWidget *parent = 0);
    ~detach_detailed_information();

private:
    Ui::detach_detailed_information *ui;

    void closeEvent(QCloseEvent *eve);
    void highlight_search_text(QString pattern);
signals:
    void close_detach_window(detach_detailed_information*);
};

#endif
