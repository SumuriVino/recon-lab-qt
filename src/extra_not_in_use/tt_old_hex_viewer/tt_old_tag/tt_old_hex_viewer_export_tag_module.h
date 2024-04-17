#ifndef EXPORT_TAG_MODULE_H
#define EXPORT_TAG_MODULE_H

#include <QDialog>
#include <QtCore>
#include <QFileDialog>

namespace Ui {
class export_tag_module;
}

class export_tag_module : public QDialog
{
    Q_OBJECT

public:
    explicit export_tag_module(QWidget *parent = 0);
    ~export_tag_module();

    void pub_clear_line_edit_text();
    void pub_set_recon_result_dir_path(QString path);
private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_export_clicked();

private:
    Ui::export_tag_module *ui;
    QFileDialog file_dialog_obj;

signals:
    void export_clicked(QString);

};

#endif // EXPORT_TAG_MODULE_H
