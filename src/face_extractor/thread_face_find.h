#ifndef thread_face_find_H
#define thread_face_find_H

#include <QThread>
#include <QDir>
#include <QFileInfo>
#include <QtCore>
#include "header_identical_1.h"
#include "header_identical_3.h"

class thread_face_find : public QThread
{
    Q_OBJECT
public:
    explicit thread_face_find();

    void run();

    bool        m_bThreadExit;
    QString     m_strSrcFilePath, m_strTargetFolderPath;
    QStringList target_path_QStringList;

    int total_rec_count = 0;

    float       m_fThreshold = 0.0f;

    void pub_set_bool_cancel_extraction(bool status);

signals:
    void        ProgressChanged(int);
    void        FaceFound(QString strFilePath);

    void signal_PBAR_thread_file_system_started(QString);
    void signal_PBAR_thread_file_system_finished(QString);
    void signal_PBAR_thread_file_system_value(QString,QString,bool,qint64,int,bool);

    void signal_finished();

    void signal_add_all_matches_to_case_tree();

private:

    int count = 0;

    bool bool_cancel_extraction = false;
};

#endif // thread_face_find_H

