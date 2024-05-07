#include "exif_data_audio_video.h"

exif_data_audio_video::exif_data_audio_video(QWidget *parent) : QWidget(parent)
{
}

exif_data_audio_video::~exif_data_audio_video()
{
}

QString exif_data_audio_video::pub_get_exif_data_audio_video(QString filepath)
{ // get exif data audio video files
   QString exif_data_str = extract_exif_data_audio_video_files(filepath);

   return exif_data_str;
}

QStringList exif_data_audio_video::pub_get_all_available_exif_data_audio_video(QString filepath)
{ // extract all audio video files from source
   QStringList exif_data_list = extract_all_available_exif_data_audio_video_files(filepath);

   return exif_data_list;
}
