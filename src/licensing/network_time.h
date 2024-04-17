#ifndef NETWORK_TIME_H
#define NETWORK_TIME_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QProcess>
#include "recon_generic_functions/recon_static_functions.h"
#include <QTcpSocket>


class network_time : public QObject
{
    Q_OBJECT
public:
    explicit network_time(QObject *parent = 0);

    bool nw_value();
    qint64 return_string();
    bool is_test_performed();
    void connect_to_nist_serv();

    void getSocialNetwork();



private slots:
    void onRequestCompleted();
    void network_error(QNetworkReply::NetworkError);
 void slot_wget_finished(int);
 void read_everything_from_socket();

 void slot_connected();
 void slot_timer();

private:
    bool network_failure;
    QString time_date;

    QNetworkAccessManager *m_networkManager;

    qint64 current_time_numeric_qint64;
    bool test_performed;

    QProcess *wget_prc;
    void run_wget();

    QTcpSocket *socket;
    QTimer *m_timer;

    bool bool_time_found;

};

#endif // NETWORK_TIME_H
