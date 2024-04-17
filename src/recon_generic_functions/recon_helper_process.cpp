#include "recon_helper_process.h"

recon_helper_process::recon_helper_process(QObject *parent) : QObject(parent)
{
    bool_process_cancel = false;
    bool_process_generic_finish = false;

    process_generic = new QProcess(this);
    connect(process_generic,SIGNAL(finished(int)),this,SLOT(slot_generic_process_finished(int)));
    prc_generic_with_argument_mergechannel = new QProcess(this);
    prc_generic_with_argument_mergechannel->setProcessChannelMode(QProcess::MergedChannels);
    connect(prc_generic_with_argument_mergechannel, SIGNAL(finished(int)), this, SLOT(slot_prc_generic_with_argument_mergechannel_finished(int)));
    connect(prc_generic_with_argument_mergechannel, SIGNAL(readyRead()), this, SLOT(slot_prc_generic_with_argument_mergechannel_readyread()));

}

recon_helper_process::~recon_helper_process()
{

}

void recon_helper_process::slot_generic_process_finished(int)
{
    bool_process_generic_finish = true;
}

QString recon_helper_process::run_command_with_arguments(QString command, QStringList arguments , QString caller_func)
{
    bool_process_generic_finish = false;
    bool_process_cancel = false;


    process_generic->start(command,arguments);
    if (!process_generic->waitForStarted())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "Error:" + process_generic->errorString() ,Q_FUNC_INFO);
        return "";
    }

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }

        if(bool_process_generic_finish || process_generic->state() != QProcess::Running)
        {
            break;
        }

        if(bool_process_cancel)
        {
            process_generic->terminate();
            break;
        }
    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}


QString recon_helper_process::run_command_by_shell(QString command, QString caller_func)
{
    bool_process_generic_finish = false;
    bool_process_cancel = false;

    process_generic->start("sh",{ "-c" , command });

    if (!process_generic->waitForStarted())
    {
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "----FAILED---- Process Start",Q_FUNC_INFO);
        recon_static_functions::app_debug(recon_static_functions::prepare_callerfun(caller_func) + "Error:" + process_generic->errorString() ,Q_FUNC_INFO);
        return "";
    }

    recon_static_functions::debug_conditional(" : running222", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }

        if(bool_process_generic_finish || process_generic->state() != QProcess::Running)
        {
            break;
        }

        if(bool_process_cancel)
        {
            process_generic->terminate();
            break;
        }
    }

    recon_static_functions::debug_conditional(" : infinite loop cross222", Q_FUNC_INFO);

    QString output = QString::fromLocal8Bit(process_generic->readAll().trimmed());

    if(output.isEmpty())
    {
        output = QString::fromLocal8Bit(process_generic->readAllStandardError().trimmed());
    }

    return output;

}


QString recon_helper_process::run_command_without_argument(QString command , QString caller_func)
{
    bool_process_generic_finish = false;
    bool_process_cancel = false;

    process_generic->start(command);

    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : running", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }

        if(bool_process_generic_finish || process_generic->state() != QProcess::Running)
        {
            break;
        }

        if(bool_process_cancel)
        {
            process_generic->terminate();
            break;
        }

    }
    recon_static_functions::debug_conditional(recon_static_functions::prepare_callerfun(caller_func) + " : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(process_generic->readAll().trimmed());
}

void recon_helper_process::slot_prc_generic_with_argument_mergechannel_finished(int)
{
    bool_prc_generic_with_argument_mergechannel_finish = true;
}

QString recon_helper_process::run_command_with_argument_mergechannel(QString command, QStringList arguments, QString caller_fun)
{
    bool_prc_generic_with_argument_mergechannel_finish = false;
    bool_process_cancel = false;

    merge_channel_prc_readall_bytearray.clear();
    prc_generic_with_argument_mergechannel->start(command,arguments);
    if(!prc_generic_with_argument_mergechannel->waitForStarted())
    {
        recon_static_functions::app_debug(" waitForStarted ---FAILED---  " + prc_generic_with_argument_mergechannel->errorString(), Q_FUNC_INFO);
        return "";
    }

    // waitForReadyRead() is used for images(like installer image),
    // which needs keyboard input for further mounting/processing..
    prc_generic_with_argument_mergechannel->waitForReadyRead(60000);



    // recon_static_functions::debug_conditional(" : running", Q_FUNC_INFO);
    int m_cnt = 0;
    while(1)
    {
        m_cnt++;
        if(m_cnt % 500 == 0)
        {
            m_cnt = 0;
            QCoreApplication::processEvents();
        }

        if(bool_prc_generic_with_argument_mergechannel_finish || (prc_generic_with_argument_mergechannel->state() == QProcess::NotRunning))
        {
            break;
        }

        if(bool_process_cancel)
        {
            prc_generic_with_argument_mergechannel->terminate();
            break;
        }

    }
    // recon_static_functions::debug_conditional(" : infinite loop cross", Q_FUNC_INFO);

    return QString::fromLocal8Bit(merge_channel_prc_readall_bytearray.trimmed());

}

void recon_helper_process::slot_prc_generic_with_argument_mergechannel_readyread()
{
    merge_channel_prc_readall_bytearray = prc_generic_with_argument_mergechannel->readAll();
    if(!merge_channel_prc_readall_bytearray.isEmpty())
    {
        merge_channel_prc_readall_bytearray.append(merge_channel_prc_readall_bytearray.trimmed());
    }
}

void recon_helper_process::pub_cancel_process()
{
    bool_process_cancel = true;
}
