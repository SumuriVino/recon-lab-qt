#import <Foundation/Foundation.h>
#import <ImageIO/ImageIO.h>

#include "exif_data/exif_data_document/exif_data_document_files.h"


QString exif_data_document_files::extract_exif_data_pdf_files(QString filepath , struct_global_exif_data_parsed_data &exif_data_parsed_obj)
{
    ///==========================================================================///

    //
    //    All the exif data is contained by infoDict in the form of CGPDFDictionaryRef
    //

    ///==========================================================================///


    NSString *target_file = filepath.toNSString();

    NSURL *fileURL = [NSURL fileURLWithPath:target_file];
    CGPDFDocumentRef document = CGPDFDocumentCreateWithURL((CFURLRef)fileURL);

    CGPDFDocumentRef pdf_ref = CGPDFDocumentRetain(document);



    QByteArray arr_output_key;
    QXmlStreamWriter xml_writer(&arr_output_key);

    xml_writer.setAutoFormatting(true);
    xml_writer.writeStartDocument();
    xml_writer.writeStartElement(QString("Exif_Data"));



    int majorVersion = 0;
    int minorVersion = 0;

    CGPDFDocumentGetVersion(pdf_ref, &majorVersion, &minorVersion);


    if(majorVersion != 0)
    {
        xml_writer.writeTextElement(QObject::tr(QString("MajorVersion").toLocal8Bit().data()),QString::number(majorVersion)
                                    + QString(MACRO_RECON_Splitter_1_multiple) + QString("Version"));
    }

    if(minorVersion != 0)
    {

        xml_writer.writeTextElement(QObject::tr(QString("MinorVersion").toLocal8Bit().data()),QString::number(minorVersion)
                                    + QString(MACRO_RECON_Splitter_1_multiple) + QString("Version"));
    }





    CGPDFDictionaryRef infoDict = CGPDFDocumentGetInfo(pdf_ref);
    CGPDFStringRef str;

    bool bool_check_exist = false;

    QStringList exif_metadata_list;
    exif_metadata_list << "MajorVersion" << "MinorVersion" << "Author" << "Subject" << "Creator" << "Producer";

    for(int i = 0 ; i < exif_metadata_list.size() ; i++)
    {
        QString tmp_str = exif_metadata_list.at(i);

        QString title;
        if (CGPDFDictionaryGetString(infoDict, tmp_str.toLocal8Bit(), &str))
        {
            NSString *title_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
            title =  QString::fromNSString(title_str);

            if(!title.isEmpty())
            {
                bool_check_exist = true;
                break;
            }
        }


    }


    if(!bool_check_exist)
        return QString("");



    /// Title
    QString title;
    if (CGPDFDictionaryGetString(infoDict, "Title", &str))
    {
        NSString *title_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        title = QString::fromNSString(title_str);


        if(!title.isEmpty())
        {

            xml_writer.writeTextElement(QObject::tr(QString("Title").toLocal8Bit().data()),title
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

        }
    }




    /// Author
    QString author;
    if (CGPDFDictionaryGetString(infoDict, "Author", &str))
    {
        NSString *author_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        author = QString::fromNSString(author_str);

        if(!author.isEmpty())
        {
            xml_writer.writeTextElement(QObject::tr(QString("Author").toLocal8Bit().data()),author
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

            exif_data_parsed_obj.exif_author = author;
        }
    }




    /// Subject
    QString subject;
    if (CGPDFDictionaryGetString(infoDict, "Subject", &str))
    {
        NSString *subject_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        subject = QString::fromNSString(subject_str);

        if(!subject.isEmpty())
        {

            xml_writer.writeTextElement(QObject::tr(QString("Subject").toLocal8Bit().data()),subject
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

        }
    }



    /// Keywords
    QString keywords;
    if (CGPDFDictionaryGetString(infoDict, "Keywords", &str))
    {
        NSString *keywords_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        keywords = QString::fromNSString(keywords_str);

        if(!keywords.isEmpty())
        {
            xml_writer.writeTextElement(QObject::tr(QString("Keywords").toLocal8Bit().data()),keywords
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
        }

    }




    /// Creator
    QString creator;
    if (CGPDFDictionaryGetString(infoDict, "Creator", &str))
    {
        NSString *creator_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        creator = QString::fromNSString(creator_str);

        if(!creator.isEmpty())
        {
            xml_writer.writeTextElement(QObject::tr(QString("Creator").toLocal8Bit().data()),creator
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
        }

    }


    /// Producer
    QString producer;
    if (CGPDFDictionaryGetString(infoDict, "Producer", &str))
    {
        NSString *producer_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        producer = QString::fromNSString(producer_str);

        if(!producer.isEmpty())
        {
            xml_writer.writeTextElement(QObject::tr(QString("Producer").toLocal8Bit().data()),producer
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
        }
    }



    /// CreationDate
    QString creation_date;
    if (CGPDFDictionaryGetString(infoDict, "CreationDate", &str))
    {
        NSDate *creation_date_value = (NSDate *)CFBridgingRelease(CGPDFStringCopyDate(str));

        NSTimeInterval ti = [creation_date_value timeIntervalSince1970];

        if(ti != 0)
        {
            NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
            NSObject *my_object = [NSString stringWithFormat:@"%f", ti];

            if(intervalString != nil && [my_object isKindOfClass:[NSString class]])
            {
                QString date_interval = QString::fromNSString(intervalString);

                if(date_interval.contains("."))
                {
                    QStringList temp = date_interval.split(".");

                    if(temp.size() > 0)
                        date_interval = temp.at(0);
                }


                creation_date = convert_numeric_timestamp_to_readable_for_display(date_interval.trimmed());

            }

        }

        if(!creation_date.isEmpty())
        {
            xml_writer.writeTextElement(QObject::tr(QString("CreationDate").toLocal8Bit().data()),creation_date
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("Timestamps"));

        }

    }



    /// ModificationDate
    QString modification_date;
    if (CGPDFDictionaryGetString(infoDict, "ModDate", &str))
    {
        NSDate *modification_date_value = (NSDate *)CFBridgingRelease(CGPDFStringCopyDate(str));

        NSTimeInterval ti = [modification_date_value timeIntervalSince1970];

        if(ti != 0)
        {
            NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
            NSObject *my_object = [NSString stringWithFormat:@"%f", ti];

            if(intervalString != nil && [my_object isKindOfClass:[NSString class]])
            {
                QString date_interval = QString::fromNSString(intervalString);

                if(date_interval.contains("."))
                {
                    QStringList temp = date_interval.split(".");

                    if(temp.size() > 0)
                        date_interval = temp.at(0);
                }


                modification_date = convert_numeric_timestamp_to_readable_for_display(date_interval.trimmed());

            }

        }

        if(!modification_date.isEmpty())
        {

            xml_writer.writeTextElement(QObject::tr(QString("ModificationDate").toLocal8Bit().data()),modification_date
                                        + QString(MACRO_RECON_Splitter_1_multiple) + QString("Timestamps"));
        }
    }


    xml_writer.writeEndElement();
    xml_writer.writeEndDocument();


    return QString::fromLocal8Bit(arr_output_key);

}

QStringList exif_data_document_files::extract_all_available_exif_data_pdf_files(QString filepath)
{
    ///==========================================================================///

    //
    //    All the exif data is contained by infoDict in the form of CGPDFDictionaryRef
    //

    ///==========================================================================///


    NSString *target_file = filepath.toNSString();

    NSURL *fileURL = [NSURL fileURLWithPath:target_file];
    CGPDFDocumentRef document = CGPDFDocumentCreateWithURL((CFURLRef)fileURL);

    CGPDFDocumentRef pdf_ref = CGPDFDocumentRetain(document);


    int majorVersion = 0;
    int minorVersion = 0;

    CGPDFDocumentGetVersion(pdf_ref, &majorVersion, &minorVersion);

    QStringList all_values_list;

    if(majorVersion != 0)
    {
        all_values_list << QString("MajorVersion") + QString(MACRO_RECON_Splitter_1_multiple) + QString::number(majorVersion);
    }

    if(minorVersion != 0)
    {
        all_values_list << QString("MinorVersion") + QString(MACRO_RECON_Splitter_1_multiple) + QString::number(minorVersion);
    }



    CGPDFDictionaryRef infoDict = CGPDFDocumentGetInfo(pdf_ref);
    CGPDFStringRef str;


    /// Title
    QString title;
    if (CGPDFDictionaryGetString(infoDict, "Title", &str))
    {
        NSString *title_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        title = QString::fromNSString(title_str);


        if(!title.isEmpty())
        {
            all_values_list << QString("Title") + QString(MACRO_RECON_Splitter_1_multiple) + title;
        }
    }




    /// Author
    QString author;
    if (CGPDFDictionaryGetString(infoDict, "Author", &str))
    {
        NSString *author_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        author = QString::fromNSString(author_str);

        if(!author.isEmpty())
        {
            all_values_list << QString("Author") + QString(MACRO_RECON_Splitter_1_multiple) + author;
        }
    }




    /// Subject
    QString subject;
    if (CGPDFDictionaryGetString(infoDict, "Subject", &str))
    {
        NSString *subject_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        subject = QString::fromNSString(subject_str);

        if(!subject.isEmpty())
        {
            all_values_list << QString("Subject") + QString(MACRO_RECON_Splitter_1_multiple) + subject;
        }
    }



    /// Keywords
    QString keywords;
    if (CGPDFDictionaryGetString(infoDict, "Keywords", &str))
    {
        NSString *keywords_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        keywords = QString::fromNSString(keywords_str);

        if(!keywords.isEmpty())
        {
            all_values_list << QString("Keywords") + QString(MACRO_RECON_Splitter_1_multiple) + keywords;
        }
    }




    /// Creator
    QString creator;
    if (CGPDFDictionaryGetString(infoDict, "Creator", &str))
    {
        NSString *creator_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        creator = QString::fromNSString(creator_str);

        if(!creator.isEmpty())
        {
            all_values_list << QString("Creator") + QString(MACRO_RECON_Splitter_1_multiple) + creator;
        }

    }


    /// Producer
    QString producer;
    if (CGPDFDictionaryGetString(infoDict, "Producer", &str))
    {
        NSString *producer_str = (NSString *)CFBridgingRelease(CGPDFStringCopyTextString(str));
        producer = QString::fromNSString(producer_str);

        if(!producer.isEmpty())
        {
            all_values_list << QString("Producer") + QString(MACRO_RECON_Splitter_1_multiple) + producer;
        }
    }



    /// CreationDate
    QString creation_date;
    if (CGPDFDictionaryGetString(infoDict, "CreationDate", &str))
    {
        NSDate *creation_date_value = (NSDate *)CFBridgingRelease(CGPDFStringCopyDate(str));

        NSTimeInterval ti = [creation_date_value timeIntervalSince1970];

        if(ti != 0)
        {
            NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
            NSObject *my_object = [NSString stringWithFormat:@"%f", ti];

            if(intervalString != nil && [my_object isKindOfClass:[NSString class]])
            {
                QString date_interval = QString::fromNSString(intervalString);

                if(date_interval.contains("."))
                {
                    QStringList temp = date_interval.split(".");

                    if(temp.size() > 0)
                        date_interval = temp.at(0);
                }


                creation_date = convert_numeric_timestamp_to_readable_for_display(date_interval.trimmed());


            }

        }

        if(!creation_date.isEmpty())
        {
            all_values_list << QString("CreationDate") + QString(MACRO_RECON_Splitter_1_multiple) + creation_date;
        }

    }



    /// ModificationDate
    QString modification_date;
    if (CGPDFDictionaryGetString(infoDict, "ModDate", &str))
    {
        NSDate *modification_date_value = (NSDate *)CFBridgingRelease(CGPDFStringCopyDate(str));

        NSTimeInterval ti = [modification_date_value timeIntervalSince1970];

        if(ti != 0)
        {
            NSString *intervalString = [NSString stringWithFormat:@"%f", ti];
            NSObject *my_object = [NSString stringWithFormat:@"%f", ti];

            if(intervalString != nil && [my_object isKindOfClass:[NSString class]])
            {
                QString date_interval = QString::fromNSString(intervalString);

                if(date_interval.contains("."))
                {
                    QStringList temp = date_interval.split(".");

                    if(temp.size() > 0)
                        date_interval = temp.at(0);
                }


                modification_date = convert_numeric_timestamp_to_readable_for_display(date_interval.trimmed());

            }

        }

        if(!modification_date.isEmpty())
        {
            all_values_list << QString("ModificationDate") + QString(MACRO_RECON_Splitter_1_multiple) + modification_date;
        }
    }


    return all_values_list;

}
