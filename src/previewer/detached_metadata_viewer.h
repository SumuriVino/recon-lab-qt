#ifndef DETACHED_METADATA_VIEWER_H
#define DETACHED_METADATA_VIEWER_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class detached_metadata_viewer;
}

class detached_metadata_viewer : public QDialog
{
    Q_OBJECT
    
public:
    explicit detached_metadata_viewer(QWidget *parent = 0);
    ~detached_metadata_viewer();
    void set_metadata_content(QString,QString);
    void set_window_title(QString);

    void highlight_search_text(QString pattern);

    
private:
    Ui::detached_metadata_viewer *ui;
};

#endif // DETACHED_METADATA_VIEWER_H
