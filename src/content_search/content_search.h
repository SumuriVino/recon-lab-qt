#ifndef CONTENT_SEARCH_H
#define CONTENT_SEARCH_H

#include <QFrame>
#include <QWidget>
#include <QFrame>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtCore>
#include <QtGui>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_identical_2.h"
#include "recon_configuration/pages_recon_configuration/recon_configuration_page_keyword_list.h"

namespace Ui {
class content_search;
}

class content_search : public QFrame
{
    Q_OBJECT

public:
    explicit content_search(QWidget *parent = 0);
    ~content_search();

    void pub_display_keyword_list();
    void pub_set_essentials();

    void pub_display_for_popup();

    void pub_clear_search_label();
signals:
    void signal_content_search_pushbutton_done_clicked(QString search_label);

public slots:
    void slot_pushButton_done_clicked();

    void slot_pushButton_cancel_clicked();


private:
    Ui::content_search *ui;
    QStackedWidget *stack_widget_main;
    QVBoxLayout *vbox_main;

    recon_configuration_page_keyword_list *recon_configuration_page_keyword_list_obj;

    void create_ui();

    QPushButton *pushButton_done;
    QPushButton *pushButton_cancel;
    QHBoxLayout *hbox_done_cancel_buttons;

    void create_done_cancel_button_box();

    QLineEdit *lineedit_search_label;



};

#endif // CONTENT_SEARCH_H
