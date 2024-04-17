#ifndef EMAIL_SEARCH_H
#define EMAIL_SEARCH_H

#include <QDialog>
#include <QFrame>
#include <QFileDialog>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>


#include "recon_generic_functions/recon_static_functions.h"
#include "header_gui_settings.h"
#include "add_tags/add_tags.h"
#include "header_global_variables_1.h"
#include "previewer/av_preview.h"
#include "previewer/image_preview.h"
#include "previewer/blank.h"
#include "header_global_variables_3.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "header_global_variables_2.h"

namespace Ui {
class email_search;
}

class email_search : public QDialog
{
    Q_OBJECT

public:
    explicit email_search(QWidget *parent = nullptr);
    ~email_search();
    void pub_load_defaults();

signals:

    void signal_email_search_work_done(QString command);

    void signal_highlight_searched_text(QString searched_text);

private slots:

    void on_pushButton_search_clicked();

    void on_checkBox_date_clicked(bool checked);

    void on_checkBox_attachments_clicked(bool checked);

    void on_comboBox_date_time_filter_currentTextChanged(const QString &arg);

private:

    Ui::email_search *ui;

    QString get_email_search_sql_predicate();

    QString get_email_subject_sql_predicate();

    QString get_email_sender_sql_predicate();

    QString get_email_receiver_sql_predicate();

    QString get_email_cc_sql_predicate();

    QString get_email_attachments_sql_predicate();

    QString get_date_filter_sql_predicate();
};

#endif // EMAIL_SEARCH_H
