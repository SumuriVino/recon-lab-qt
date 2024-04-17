#ifndef EXIF_DATA_AUDIO_VIDEO_H
#define EXIF_DATA_AUDIO_VIDEO_H

#include <QWidget>
#include <QtCore>

#include "header_identical_1.h"


class exif_data_audio_video : public QWidget
{
    Q_OBJECT
public:
    explicit exif_data_audio_video(QWidget *parent = 0);
    ~exif_data_audio_video();

    QString pub_get_exif_data_audio_video(QString filepath);

    QStringList pub_get_all_available_exif_data_audio_video(QString filepath);
signals:

public slots:

private:
    QString extract_exif_data_audio_video_files(QString filepath);



    QStringList extract_all_available_exif_data_audio_video_files(QString filepath);
};

#endif // EXIF_DATA_AUDIO_VIDEO_H
