#include <QtCore>
#include <QHostAddress>
#include <QPushButton>
#include <CoreServices/CoreServices.h>
#include "rcbuild/build_items_common_for_recon_and_artifact.h"
#include "rcbuild/ios_uid_backup_parser.h"

#define MACRO_GLOBAL_ARTIFACT_RUN_BY_RECON

#define MACRO_GLOBAL_ARTIFACT_NILPOWER_int -1
#define MACRO_GLOBAL_ARTIFACT_NILPOWER_QSTRING "NILSTR"


#define MACRO_GLOBAL_ARTIFACT_Static_Functions_Size_KB_int 1024
#define MACRO_GLOBAL_ARTIFACT_Static_Functions_Size_MB_int 1024*1024
#define MACRO_GLOBAL_ARTIFACT_Static_Functions_Size_GB_int 1024*1024*1024



struct struct_GLOBAL_ARTIFACT_cfstring_attributes
{
    QString attr_date_added_numeric_QString;
    QString attr_content_creation_date_numeric_QString;
    QString attr_content_modification_date_numeric_QString;
    QString attr_content_type;
    QString attr_kind;
    QString attr_fs_content_change_date_numeric_QString;
    QString attr_fs_creation_date_numeric_QString;
    QString attr_fs_name;
    QString attr_fs_node_count_numeric_QString;
    QString attr_wherefroms;
    QString attr_latitude_numeric_QString;
    QString attr_longitude_numeric_QString;
    QString attr_altitude_numeric_QString;
    QString attr_torrent_files_list;
    QString attr_duration;
    QString attr_file_type;
    QString attr_message_id;
    QString attr_sender_display_name;
    QString attr_sender_handle_and_email_id;
    QString attr_mail_subject;
    QString attr_composer;
    QString attr_author;
    QString attr_gps_datestamp;
    QString attr_orientation;
    QString attr_pixel_count;
    QString attr_pixel_height;
    QString attr_pixel_width;
    QString attr_device_make;
    QString attr_device_model;
    QString attr_bit_per_sample;
    QString attr_color_space;
    QString attr_audio_bit_rate;
    QString attr_audio_channel_count;
    QString attr_codecs;
    QString attr_video_bit_rate;
    QString attr_total_bit_rate;
    QString attr_duration_second_numeric_QString;
};

struct struct_GLOBAL_ARTIFACT_used_dates
{
    QString last_used_date_numeric_QString;

    QStringList all_used_dates_numeric_QStringList;
    QString first_date_from_all_used_dates_numeric_QString;
    QString last_date_from_all_used_dates_numeric_QString;

    int sanity_check_dates_int;
};




struct struct_GLOBAL_ARTIFACT_extended_attributes_key_values
{
        QString values_QString;
        QString key_value_pair_QString;

};

struct struct_GLOBAL_ARTIFACT_artifact_source_file
{
    QString artifacts_source_file;
    QString artifacts_relation_source_file;
};
