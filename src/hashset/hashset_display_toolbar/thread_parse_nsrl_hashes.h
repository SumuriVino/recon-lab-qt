#ifndef THREAD_PARSE_NSRL_HASHES_H
#define THREAD_PARSE_NSRL_HASHES_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "recon_generic_functions/recon_helper_standard.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "narad_muni/narad_muni.h"


class thread_parse_nsrl_hashes : public QThread
{
    Q_OBJECT

public:
    thread_parse_nsrl_hashes(QObject *parent = nullptr);
    ~thread_parse_nsrl_hashes();

    void run();
    void pub_set_essentials(QString filepath, QString dest_db_path);

    void pub_cancel_extraction();
private slots:

signals:
   void signal_nsrl_parse_status(qint64 records, int prog_status);

private:

   recon_helper_standard *recon_helper_standard_obj;
    bool bool_cancel_loading;
    QString nsrl_filepath;
    QString destination_db_filepath;

};

#endif // THREAD_PARSE_NSRL_HASHES_H
