#ifndef DETACHED_METADATA_VIEWER_module_H
#define DETACHED_METADATA_VIEWER_module_H

#include <QDialog>
#include <QtCore>

namespace Ui {
class detached_metadata_viewer_module;
}

class detached_metadata_viewer_module : public QDialog
{
    Q_OBJECT
    
public:
    explicit detached_metadata_viewer_module(const QString &title, QWidget *parent = 0);
    ~detached_metadata_viewer_module();
    void set_metadata_content(QString ,QString);
        void highlight_search_text(QString pattern);

        void set_window_title(QString);

private:
    Ui::detached_metadata_viewer_module *ui;
    QString w_title;
};

#endif // DETACHED_METADATA_VIEWER_H
