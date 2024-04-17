#ifndef RECON_HELPER_PROCESS_H
#define RECON_HELPER_PROCESS_H

#include <QObject>
#include "recon_static_functions.h"

class recon_helper_process : public QObject
{
    Q_OBJECT
public:
    explicit recon_helper_process(QObject *parent = nullptr);
    ~recon_helper_process();

    QString run_command_with_arguments(QString command, QStringList arguments, QString caller_func);
    QString run_command_without_argument(QString command, QString caller_func);
    QString run_command_with_argument_mergechannel(QString command, QStringList arguments, QString caller_fun);
    QString run_command_by_shell(QString command, QString caller_func);

    void pub_cancel_process();


signals:

private slots:
    void slot_generic_process_finished(int);
    void slot_prc_generic_with_argument_mergechannel_readyread();
    void slot_prc_generic_with_argument_mergechannel_finished(int);

private:

    QProcess *process_generic;
    bool bool_process_generic_finish;
    bool bool_prc_generic_with_argument_mergechannel_finish;
    bool bool_process_cancel;

    QProcess *prc_generic_with_argument_mergechannel;
    QByteArray merge_channel_prc_readall_bytearray;

};

#endif // RECON_HELPER_PROCESS_H
