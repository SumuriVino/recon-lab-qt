#ifndef HEX_VIEWER_SEARCH_DIALOG_H
#define HEX_VIEWER_SEARCH_DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QMessageBox>
#include "recon_generic_functions/recon_static_functions.h"

namespace Ui {
class hex_viewer_search_dialog;
}

class hex_viewer_search_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit hex_viewer_search_dialog(QWidget *parent = 0);
    ~hex_viewer_search_dialog();

    void pub_highlight_keyword(QString keyword_str);

signals:
    void signal_search_text_and_type(QString search_text, QString search_type, bool bool_clear_prev_list);

private slots:
    void on_pushButton_search_clicked();

private:
    Ui::hex_viewer_search_dialog *ui;
};

#endif // HEX_VIEWER_SEARCH_DIALOG_H
