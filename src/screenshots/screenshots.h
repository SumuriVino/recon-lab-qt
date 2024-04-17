#ifndef SCREENSHOTS_H
#define SCREENSHOTS_H

#include <QFrame>
#include <QtCore>
                                   
#include "recon_generic_functions/recon_static_functions.h"
#include "previewer/image_preview.h"
#include <QFileDialog>
#include "header_global_variables_2.h"

namespace Ui {
class screenshots;
}

class screenshots : public QFrame
{
    Q_OBJECT

public:
    explicit screenshots(QWidget *parent = 0);
    ~screenshots();

    void closeEvent(QCloseEvent *);

    void set_screenshot();

    void clear_fields();

    bool pub_screenshot_show();
private slots:
    void slot_generic_process_finished(int);
    void on_pushButton_dir_path_clicked();

    void on_pushButton_done_clicked();

    void on_pushButton_cancel_clicked();

signals:

    void signal_screenshots_work_complete(QString filepath, QString comment, QString tags);

    void signal_screenshots_canceled();


private:
    Ui::screenshots *ui;

    QString images_dir;

    QString run_command_generic(QString command, QStringList arguments);
    QProcess *process_generic;
    bool bool_process_generic;

    QClipboard *m_clipboard;
    void set_gui();

    image_preview *image_previewe_obj ;


};

#endif //
