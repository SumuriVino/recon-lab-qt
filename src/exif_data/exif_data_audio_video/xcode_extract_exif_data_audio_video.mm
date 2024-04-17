#import <Foundation/Foundation.h>

#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAsset.h>

#include "exif_data/exif_data_audio_video/exif_data_audio_video.h"


QString exif_data_audio_video::extract_exif_data_audio_video_files(QString filepath)
{

    ///==========================================================================///

    //
    //    All the exif data is contained by commonMetadata in the form of NSArray as
    //
    //       "<AVMetadataItem: 0x7f9d5e5301c0, identifier=id3/TIT2, keySpace=org.id3, key class = NSTaggedPointerString, key=TIT2, commonKey=title, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Hamari Adhuri Kahani - Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e530470, identifier=id3/TCOM, keySpace=org.id3, key class = NSTaggedPointerString, key=TCOM, commonKey=creator, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Music: Ami Mishra | Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e52d730, identifier=id3/TIT1, keySpace=org.id3, key class = NSTaggedPointerString, key=TIT1, commonKey=subject, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e522800, identifier=id3/TIT3, keySpace=org.id3, key class = NSTaggedPointerString, key=TIT3, commonKey=description, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e51ba40, identifier=id3/TPUB, keySpace=org.id3, key class = NSTaggedPointerString, key=TPUB, commonKey=publisher, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e533090, identifier=id3/TOPE, keySpace=org.id3, key class = NSTaggedPointerString, key=TOPE, commonKey=contributor, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Music: Ami Mishra | Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e5312a0, identifier=id3/TCON, keySpace=org.id3, key class = NSTaggedPointerString, key=TCON, commonKey=type, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Bollywood Music>",
    //       "<AVMetadataItem: 0x7f9d5e531dc0, identifier=id3/TCOP, keySpace=org.id3, key class = NSTaggedPointerString, key=TCOP, commonKey=copyrights, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e531ab0, identifier=id3/TALB, keySpace=org.id3, key class = NSTaggedPointerString, key=TALB, commonKey=albumName, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Hamari Adhuri Kahani (2015)>",
    //       "<AVMetadataItem: 0x7f9d5e5306a0, identifier=id3/TEXT, keySpace=org.id3, key class = NSTaggedPointerString, key=TEXT, commonKey=author, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e5317e0, identifier=id3/TPE1, keySpace=org.id3, key class = NSTaggedPointerString, key=TPE1, commonKey=artist, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Arijit Singh>",
    //       "<AVMetadataItem: 0x7f9d5e532130, identifier=id3/APIC, keySpace=org.id3, key class = NSTaggedPointerString, key=APIC, commonKey=artwork, extendedLanguageTag=(null), dataType=com.apple.metadata.datatype.JPEG, time={INVALID}, duration={INVALID}, startDate=(null), extras={\n    dataType = \"image/jpeg\";\n    dataTypeNamespace = \"org.iana.media-type\";\n    info = \"\";\n    pictureType = \"Cover (front)\";\n}, value length=68670>"

    ///==========================================================================///


    NSString *target_file = filepath.toNSString();

    NSURL *fileURL = [NSURL fileURLWithPath:target_file];

    AVAsset *assest = [AVURLAsset URLAssetWithURL:fileURL options:nil];


    ///**********************common metadata**********************


    NSArray *commonMetadata = [assest commonMetadata];

    bool bool_is_metadata_key_exist = false;

    QStringList exif_metadata_list;
    exif_metadata_list << "common/title" << "common/creator" << "common/subject" << "common/publisher" << "common/contributor" << "common/author";

    if ([commonMetadata count] > 0)
    {
        for (AVMetadataItem *item in commonMetadata)
        {

            NSString *commonIdentifier = [AVMetadataItem identifierForKey:[item commonKey] keySpace:AVMetadataKeySpaceCommon];

            QString common_metadata_key   = QString::fromNSString(commonIdentifier);


            if(exif_metadata_list.contains(common_metadata_key))
            {

                bool_is_metadata_key_exist = true;
                break;

            }

        }

    }

    if(!bool_is_metadata_key_exist)
        return QString("");


    if ([commonMetadata count] > 0)
    {

        QByteArray arr_output_key;
        QXmlStreamWriter xml_writer(&arr_output_key);

        xml_writer.setAutoFormatting(true);
        xml_writer.writeStartDocument();
        xml_writer.writeStartElement(QString("Exif_Data"));



        for (AVMetadataItem *item in commonMetadata)
        {
            NSString *commonIdentifier = [AVMetadataItem identifierForKey:[item commonKey] keySpace:AVMetadataKeySpaceCommon];

            QString common_metadata_key   = QString::fromNSString(commonIdentifier);
            QString common_metadata_value = QString::fromNSString([item stringValue]);


            /// title
            QString title;
            if(common_metadata_key == QString("common/title"))
            {
                title = common_metadata_value;

                if(!title.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("title").toLocal8Bit().data()),title
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }


            }

            /// creator
            QString creator;
            if(common_metadata_key == QString("common/creator"))
            {
                creator = common_metadata_value;

                if(!creator.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("creator").toLocal8Bit().data()),creator
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }
            }

            /// subject
            QString subject;
            if(common_metadata_key == QString("common/subject"))
            {
                subject = common_metadata_value;

                if(!subject.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("subject").toLocal8Bit().data()),subject
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }

            }

            /// description
            QString description;
            if(common_metadata_key == QString("common/description"))
            {
                description = common_metadata_value;

                if(!description.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("description").toLocal8Bit().data()),description
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
                }
            }

            /// publisher
            QString publisher;
            if(common_metadata_key == QString("common/publisher"))
            {
                publisher = common_metadata_value;

                if(!publisher.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("publisher").toLocal8Bit().data()),description
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }
            }

            /// contributor
            QString contributor;
            if(common_metadata_key == QString("common/contributor"))
            {
                contributor = common_metadata_value;

                if(!contributor.isEmpty())
                {
                    xml_writer.writeTextElement(QObject::tr(QString("contributor").toLocal8Bit().data()),contributor
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }
            }

            /// type
            QString type;
            if(common_metadata_key == QString("common/type"))
            {
                type = common_metadata_value;

                if(!type.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("type").toLocal8Bit().data()),type
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }
            }

            /// copyrights
            QString copyrights;
            if(common_metadata_key == QString("common/copyrights"))
            {
                copyrights = common_metadata_value;

                if(!copyrights.isEmpty())
                {
                    xml_writer.writeTextElement(QObject::tr(QString("copyrights").toLocal8Bit().data()),copyrights
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
                }
            }

            /// albumName
            QString albumName;
            if(common_metadata_key == QString("common/albumName"))
            {
                albumName = common_metadata_value;

                if(!albumName.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("albumName").toLocal8Bit().data()),albumName
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }
            }

            /// author
            QString author;
            if(common_metadata_key == QString("common/author"))
            {
                author = common_metadata_value;

                if(!author.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("author").toLocal8Bit().data()),author
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
                }
            }

            /// artist
            QString artist;
            if(common_metadata_key == QString("common/artist"))
            {
                artist = common_metadata_value;

                if(!artist.isEmpty())
                {
                    xml_writer.writeTextElement(QObject::tr(QString("artist").toLocal8Bit().data()),artist
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }
            }

            /// artwork
            QString artwork;
            if(common_metadata_key == QString("common/artwork"))
            {
                artwork = common_metadata_value;

                if(!artwork.isEmpty())
                {

                    xml_writer.writeTextElement(QObject::tr(QString("artwork").toLocal8Bit().data()),artwork
                                                + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                }

            }


        }

        xml_writer.writeEndElement();
        xml_writer.writeEndDocument();


        return QString::fromLocal8Bit(arr_output_key);


    }
    else
        return QString("");






    /*
     * =======================
     * Do Not Remove this code
     * =======================
     */


    ///**********************all available metadata**********************

    //    for (NSString *format in [assest availableMetadataFormats])
    //    {
    //        NSArray *items = [assest metadataForFormat:format];
    //        if ([items count] > 0)
    //        {
    //            for (AVMetadataItem *item in items)
    //            {
    //                NSString *identifier = [item identifier];
    //                id value = [item value];

    //                if ([value isKindOfClass:[NSString class]])
    //                {
    //                    QString common_metadata_key   = [identifier UTF8String];
    //                    QString common_metadata_value = [[item stringValue] UTF8String];



    //                }

    //            }
    //        }
    //    }



}

QStringList exif_data_audio_video::extract_all_available_exif_data_audio_video_files(QString filepath)
{

    ///==========================================================================///

    //
    //    All the exif data is contained by commonMetadata in the form of NSArray as
    //
    //       "<AVMetadataItem: 0x7f9d5e5301c0, identifier=id3/TIT2, keySpace=org.id3, key class = NSTaggedPointerString, key=TIT2, commonKey=title, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Hamari Adhuri Kahani - Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e530470, identifier=id3/TCOM, keySpace=org.id3, key class = NSTaggedPointerString, key=TCOM, commonKey=creator, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Music: Ami Mishra | Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e52d730, identifier=id3/TIT1, keySpace=org.id3, key class = NSTaggedPointerString, key=TIT1, commonKey=subject, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e522800, identifier=id3/TIT3, keySpace=org.id3, key class = NSTaggedPointerString, key=TIT3, commonKey=description, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e51ba40, identifier=id3/TPUB, keySpace=org.id3, key class = NSTaggedPointerString, key=TPUB, commonKey=publisher, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e533090, identifier=id3/TOPE, keySpace=org.id3, key class = NSTaggedPointerString, key=TOPE, commonKey=contributor, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Music: Ami Mishra | Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e5312a0, identifier=id3/TCON, keySpace=org.id3, key class = NSTaggedPointerString, key=TCON, commonKey=type, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Bollywood Music>",
    //       "<AVMetadataItem: 0x7f9d5e531dc0, identifier=id3/TCOP, keySpace=org.id3, key class = NSTaggedPointerString, key=TCOP, commonKey=copyrights, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e531ab0, identifier=id3/TALB, keySpace=org.id3, key class = NSTaggedPointerString, key=TALB, commonKey=albumName, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Hamari Adhuri Kahani (2015)>",
    //       "<AVMetadataItem: 0x7f9d5e5306a0, identifier=id3/TEXT, keySpace=org.id3, key class = NSTaggedPointerString, key=TEXT, commonKey=author, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Songspk.LINK>",
    //       "<AVMetadataItem: 0x7f9d5e5317e0, identifier=id3/TPE1, keySpace=org.id3, key class = NSTaggedPointerString, key=TPE1, commonKey=artist, extendedLanguageTag=(null), dataType=(null), time={INVALID}, duration={INVALID}, startDate=(null), extras={\n}, value=Arijit Singh>",
    //       "<AVMetadataItem: 0x7f9d5e532130, identifier=id3/APIC, keySpace=org.id3, key class = NSTaggedPointerString, key=APIC, commonKey=artwork, extendedLanguageTag=(null), dataType=com.apple.metadata.datatype.JPEG, time={INVALID}, duration={INVALID}, startDate=(null), extras={\n    dataType = \"image/jpeg\";\n    dataTypeNamespace = \"org.iana.media-type\";\n    info = \"\";\n    pictureType = \"Cover (front)\";\n}, value length=68670>"

    ///==========================================================================///

    NSString *target_file = filepath.toNSString();

    NSURL *fileURL = [NSURL fileURLWithPath:target_file];

    AVAsset *assest = [AVURLAsset URLAssetWithURL:fileURL options:nil];



    ///**********************all available metadata**********************

    QStringList all_available_list;
    for (NSString *format in [assest availableMetadataFormats])
    {
        NSArray *items = [assest metadataForFormat:format];
        if ([items count] > 0)
        {
            for (AVMetadataItem *item in items)
            {
                NSString *identifier = [item identifier];
                id value = [item value];

                if ([value isKindOfClass:[NSString class]])
                {
                    QString common_metadata_key   = [identifier UTF8String];
                    QString common_metadata_value = [[item stringValue] UTF8String];


                    all_available_list << common_metadata_key + QString(MACRO_RECON_Splitter_1_multiple) + common_metadata_value;
                }

            }
        }
    }

    return all_available_list;



}
