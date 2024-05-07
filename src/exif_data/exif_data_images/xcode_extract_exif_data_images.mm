#import <Foundation/Foundation.h>
#import <ImageIO/ImageIO.h>

#include "exif_data_image_files.h"


QString exif_data_image_files::extract_exif_data_image_files(QString file_path , struct_global_exif_data_parsed_data &exif_data_parsed_obj)
{ // extract image files

    ///==========================================================================///

    //
    // All the exif data is contained by exif_dict in the form of dictionary as
    //

    //    ColorModel = RGB;
    //       DPIHeight = 72;
    //       DPIWidth = 72;
    //       Depth = 8;
    //       Orientation = 6;
    //       PixelHeight = 2448;
    //       PixelWidth = 3264;
    //       ProfileName = "sRGB IEC61966-2.1";
    //       "{ExifAux}" =     {
    //           Regions =         {
    //               HeightAppliedTo = 2448;
    //               RegionList =             (
    //                                   {
    //                       AngleInfoRoll = 270;
    //                       AngleInfoYaw = 0;
    //                       ConfidenceLevel = 1000;
    //                       FaceID = 5;
    //                       Height = "0.079258";
    //                       Timestamp = 2147483647;
    //                       Type = Face;
    //                       Width = "0.059145";
    //                       X = "0.339107";
    //                       Y = "0.360044";
    //                   }
    //               );
    //               WidthAppliedTo = 3264;
    //           };
    //       };
    //       "{Exif}" =     {
    //           ApertureValue = "2.52606882168926";
    //           BrightnessValue = "4.135244793075467";
    //           ColorSpace = 1;
    //           ComponentsConfiguration =         (
    //               1,
    //               2,
    //               3,
    //               0
    //           );
    //           DateTimeDigitized = "2016:09:16 06:51:25";
    //           DateTimeOriginal = "2016:09:16 06:51:25";
    //           ExifVersion =         (
    //               2,
    //               2,
    //               1
    //           );
    //           ExposureBiasValue = 0;
    //           ExposureMode = 0;
    //           ExposureProgram = 2;
    //           ExposureTime = "0.0303030303030303";
    //           FNumber = "2.4";
    //           Flash = 32;
    //           FlashPixVersion =         (
    //               1,
    //               0
    //           );
    //           FocalLenIn35mmFilm = 31;
    //           FocalLength = "3.3";
    //           ISOSpeedRatings =         (
    //               32
    //           );
    //           LensMake = Apple;
    //           LensModel = "iPad mini 4 back camera 3.3mm f/2.4";
    //           LensSpecification =         (
    //               "3.3",
    //               "3.3",
    //               "2.4",
    //               "2.4"
    //           );
    //           MeteringMode = 5;
    //           PixelXDimension = 3264;
    //           PixelYDimension = 2448;
    //           SceneCaptureType = 0;
    //           SceneType = 1;
    //           SensingMethod = 2;
    //           ShutterSpeedValue = "5.058989898989899";
    //           SubjectArea =         (
    //               1631,
    //               1223,
    //               1795,
    //               1077
    //           );
    //           SubsecTimeDigitized = 850;
    //           SubsecTimeOriginal = 850;
    //           WhiteBalance = 0;
    //       };
    //       "{MakerApple}" =     {
    //           1 = 4;
    //           14 = 0;
    //           2 = <16001b00 27002000 5e00dc00 57006f00 16000d00 09000500 02000000 00000000 15001300 26008200 1c011a02 5f018800 22001200 0d000900 04000200 00000000 15001f00 90003300 6d006b02 0102a701 63001700 11000d00 07000300 01000000 18007800 21002000 49001602 ac017901 72001c00 13000f00 0d000600 03000000 3e006e00 1c003200 1d00be00 7a01bc00 8c00b600 93005900 2a000e00 05000200 80008500 16002600 43006300 ec00dc00 63011501 d5007a00 2d001600 08000300 86008d00 18002d00 2a008d00 1e017101 50013001 fe00c100 79003f00 11000600 8a009f00 19005100 4b00f601 bf013b01 0101d600 9c003100 2c002c00 16000800 8d00a700 38003800 53002a02 d7019f01 f2003200 e5000f00 1b002900 1b000a00 8d008800 17003c00 36002902 7d02fd01 0b013800 d4001000 1f002f00 20000e00 8d006e00 15003000 26002302 ad02eb01 0e013100 c3002100 1f003400 24001000 8d006d00 14002300 2500e601 1c02d501 0d012800 2c000b00 1b003800 29001300 8b009100 24004100 2600bf01 9401bf01 0a012400 0c000900 19003900 29001400 86009b00 57004200 30009501 8101a701 f0001f00 0a000a00 19003d00 2e001600 80007300 83007e00 34007f01 c8010201 5c001900 0a001200 24004000 31001500 58004f00 47004900 b7009001 5d015700 76001400 0a001e00 36004400 31001600>;
    //           20 = 4;
    //           3 =         {
    //               epoch = 0;
    //               flags = 1;
    //               timescale = 1000000000;
    //               value = 2613356245333;
    //           };
    //           4 = 1;
    //           5 = 143;
    //           6 = 131;
    //           7 = 1;
    //           8 =         (
    //               "0.008843093",
    //               "-0.9765947",
    //               "-0.197011"
    //           );
    //           9 = 275;
    //       };
    //       "{TIFF}" =     {
    //           DateTime = "2016:09:16 06:51:25";
    //           Make = Apple;
    //           Model = "iPad mini 4";
    //           Orientation = 6;
    //           ResolutionUnit = 2;
    //           Software = "10.0.1";
    //           XResolution = 72;
    //           YResolution = 72;
    //       };
    //   }

    ///==========================================================================///





    NSString *target_file = file_path.toNSString();


    NSDictionary* exif_dict   =   nil;
    NSURL       * url         =   [ NSURL fileURLWithPath : target_file ];

    if (url != NULL)
    {
        CGImageSourceRef source = CGImageSourceCreateWithURL ( (CFURLRef) url, NULL);

        if(source != NULL)
        {
            CFDictionaryRef metadataRef = CGImageSourceCopyPropertiesAtIndex ( source, 0, NULL );
            if ( metadataRef )
            {
                NSDictionary* immutableMetadata = (__bridge NSDictionary *)metadataRef;

                if (immutableMetadata)
                {
                    exif_dict = [NSDictionary dictionaryWithDictionary : (__bridge NSDictionary *)metadataRef];
                }

                CFRelease ( metadataRef );
            }


            CFRelease(source);
            source = nil;
        }
    }

    bool bool_check_exist = false;
    all_keys_list.clear();
    QStringList exif_metadata_list;
    exif_metadata_list << "XResolution" << "YResolution" << "Model" << "Latitude" << "Longitude" << "Altitude";

    check_metadata_keys_exist_in_nsdictionary(exif_dict);

    //    for(int i = 0; i < all_keys_list.size() ; i++)
    //    {
    //        if(exif_metadata_list.contains(all_keys_list.at(i)))
    //        {
    //            bool_check_exist = true;
    //            break;
    //        }
    //    }


    //    if(!bool_check_exist)
    //        return QString("");


    if (exif_dict != NULL)
    {

        QByteArray arr_output_key;
        QXmlStreamWriter xml_writer(&arr_output_key);

        xml_writer.setAutoFormatting(true);
        xml_writer.writeStartDocument();
        xml_writer.writeStartElement(QString("Exif_Data"));


        NSObject* type_check_obj;


        ///ColorModel
        QString ColorModel;
        NSString* ColorModel_key = @"ColorModel";
        NSString* ColorModel_value = [exif_dict objectForKey:ColorModel_key];
        type_check_obj = [exif_dict objectForKey:ColorModel_key];
        if(ColorModel_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            ColorModel = QString::fromNSString(ColorModel_value);

            if(!ColorModel.isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ColorModel").toLocal8Bit().data()),ColorModel
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
            }

        }


        ///DPIHeight
        double    DPIHeight = 0;
        NSString* DPIHeight_key = @"DPIHeight";
        NSNumber* DPIHeight_value = [exif_dict objectForKey:DPIHeight_key];
        type_check_obj = [exif_dict objectForKey:DPIHeight_key];
        if(DPIHeight_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            DPIHeight = [DPIHeight_value doubleValue];

            if(!QString::number(DPIHeight).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("DPIHeight").toLocal8Bit().data()),QString::number(DPIHeight)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
            }

        }

        ///DPIWidth
        double    DPIWidth = 0;
        NSString* DPIWidth_key = @"DPIWidth";
        NSNumber* DPIWidth_value = [exif_dict objectForKey:DPIWidth_key];
        type_check_obj = [exif_dict objectForKey:DPIWidth_key];
        if(DPIWidth_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            DPIWidth = [DPIWidth_value doubleValue];

            if(!QString::number(DPIWidth).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("DPIWidth").toLocal8Bit().data()),QString::number(DPIWidth)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

            }

        }

        ///Depth
        double    Depth = 0;
        NSString* Depth_key = @"Depth";
        NSNumber* Depth_value = [exif_dict objectForKey:Depth_key];
        type_check_obj = [exif_dict objectForKey:Depth_key];
        if(Depth_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            Depth = [Depth_value doubleValue];
            if(!QString::number(Depth).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("Depth").toLocal8Bit().data()),QString::number(Depth)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
            }

        }

        ///Orientation
        double    Orientation = 0;
        NSString* Orientation_key = @"Orientation";
        NSNumber* Orientation_value = [exif_dict objectForKey:Orientation_key];
        type_check_obj = [exif_dict objectForKey:Orientation_key];
        if(Orientation_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            Orientation = [Orientation_value doubleValue];

            if(!QString::number(Orientation).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("Orientation").toLocal8Bit().data()),QString::number(Orientation)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

            }

        }

        ///PixelHeight
        double    PixelHeight = 0;
        NSString* PixelHeight_key = @"PixelHeight";
        NSNumber* PixelHeight_value = [exif_dict objectForKey:PixelHeight_key];
        type_check_obj = [exif_dict objectForKey:PixelHeight_key];
        if(PixelHeight_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            PixelHeight = [PixelHeight_value doubleValue];

            if(!QString::number(PixelHeight).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("PixelHeight").toLocal8Bit().data()),QString::number(PixelHeight)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Pixel Details"));
            }

        }

        ///PixelWidth
        double    PixelWidth = 0;
        NSString* PixelWidth_key = @"PixelWidth";
        NSNumber* PixelWidth_value = [exif_dict objectForKey:PixelWidth_key];
        type_check_obj = [exif_dict objectForKey:PixelWidth_key];
        if(PixelWidth_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            PixelWidth = [PixelWidth_value doubleValue];

            if(!QString::number(PixelWidth).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("PixelWidth").toLocal8Bit().data()),QString::number(PixelWidth)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Pixel Details"));
            }
        }

        ///ProfileName
        QString   ProfileName;
        NSString* ProfileName_key = @"ProfileName";
        NSString* ProfileName_value = [exif_dict objectForKey:ProfileName_key];
        type_check_obj = [exif_dict objectForKey:ProfileName_key];
        if(ProfileName_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            ProfileName = QString::fromNSString(ProfileName_value);

            if(!ProfileName.isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ProfileName").toLocal8Bit().data()),ProfileName
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));
            }
        }




        /// ********************real exif chunk**********************

        id exifTree = [exif_dict objectForKey:@"{Exif}"];



        /// ApertureValue
        double    ApertureValue = 0;
        NSString* ApertureValue_key = @"ApertureValue";
        NSNumber* ApertureValue_value = [exifTree objectForKey:ApertureValue_key];
        type_check_obj = [exif_dict objectForKey:ApertureValue_key];
        if(ApertureValue_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ApertureValue = [ApertureValue_value doubleValue];

            if(!QString::number(ApertureValue).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ApertureValue").toLocal8Bit().data()),ProfileName
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));

            }
        }

        /// BrightnessValue
        double    BrightnessValue = 0;
        NSString* BrightnessValue_key = @"BrightnessValue";
        NSNumber* BrightnessValue_value = [exifTree objectForKey:BrightnessValue_key];
        type_check_obj = [exif_dict objectForKey:BrightnessValue_key];
        if(BrightnessValue_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            BrightnessValue = [BrightnessValue_value doubleValue];

            if(!QString::number(BrightnessValue).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("BrightnessValue").toLocal8Bit().data()),QString::number(BrightnessValue)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }


        /// ColorSpace
        double    ColorSpace = 0;
        NSString* ColorSpace_key = @"ColorSpace";
        NSNumber* ColorSpace_value = [exifTree objectForKey:ColorSpace_key];
        type_check_obj = [exif_dict objectForKey:ColorSpace_key];
        if(ColorSpace_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ColorSpace = [ColorSpace_value doubleValue];

            if(!QString::number(ColorSpace).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ColorSpace").toLocal8Bit().data()),QString::number(ColorSpace)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }

        /// DateTimeDigitized
        double    DateTimeDigitized = 0;
        NSString* DateTimeDigitized_key = @"DateTimeDigitized";
        NSNumber* DateTimeDigitized_value = [exifTree objectForKey:DateTimeDigitized_key];
        type_check_obj = [exif_dict objectForKey:DateTimeDigitized_key];
        if(DateTimeDigitized_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            DateTimeDigitized = [DateTimeDigitized_value doubleValue];

            if(!QString::number(DateTimeDigitized).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("DateTimeDigitized").toLocal8Bit().data()),QString::number(DateTimeDigitized)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("DateTime Details"));
            }
        }


        /// DateTimeOriginal
        double    DateTimeOriginal = 0;
        NSString* DateTimeOriginal_key = @"DateTimeOriginal";
        NSNumber* DateTimeOriginal_value = [exifTree objectForKey:DateTimeOriginal_key];
        type_check_obj = [exif_dict objectForKey:DateTimeOriginal_key];
        if(DateTimeOriginal_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            DateTimeOriginal = [DateTimeOriginal_value doubleValue];

            if(!QString::number(DateTimeOriginal).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("DateTimeOriginal").toLocal8Bit().data()),QString::number(DateTimeOriginal)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("DateTime Details"));
            }
        }





        /// ExposureBiasValue
        double    ExposureBiasValue = 0;
        NSString* ExposureBiasValue_key = @"ExposureBiasValue";
        NSNumber* ExposureBiasValue_value = [exifTree objectForKey:ExposureBiasValue_key];
        type_check_obj = [exif_dict objectForKey:ExposureBiasValue_key];
        if(ExposureBiasValue_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ExposureBiasValue = [ExposureBiasValue_value doubleValue];

            if(!QString::number(ExposureBiasValue).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ExposureBiasValue").toLocal8Bit().data()),QString::number(ExposureBiasValue)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));

            }
        }


        /// ExposureMode
        double    ExposureMode = 0;
        NSString* ExposureMode_key = @"ExposureMode";
        NSNumber* ExposureMode_value = [exifTree objectForKey:ExposureMode_key];
        type_check_obj = [exif_dict objectForKey:ExposureMode_key];
        if(ExposureMode_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ExposureMode = [ExposureMode_value doubleValue];

            if(!QString::number(ExposureMode).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ExposureMode").toLocal8Bit().data()),QString::number(ExposureMode)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }


        /// ExposureProgram
        double    ExposureProgram = 0;
        NSString* ExposureProgram_key = @"ExposureProgram";
        NSNumber* ExposureProgram_value = [exifTree objectForKey:ExposureProgram_key];
        type_check_obj = [exif_dict objectForKey:ExposureProgram_key];
        if(ExposureProgram_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ExposureProgram = [ExposureProgram_value doubleValue];

            if(!QString::number(ExposureProgram).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("ExposureProgram").toLocal8Bit().data()),QString::number(ExposureProgram)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }



        /// ExposureTime
        double    ExposureTime = 0;
        NSString* ExposureTime_key = @"ExposureTime";
        NSNumber* ExposureTime_value = [exifTree objectForKey:ExposureTime_key];
        type_check_obj = [exif_dict objectForKey:ExposureTime_key];
        if(ExposureTime_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ExposureTime = [ExposureTime_value doubleValue];

            if(!QString::number(ExposureTime).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("ExposureTime").toLocal8Bit().data()),QString::number(ExposureTime)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }


        /// FNumber
        double    FNumber = 0;
        NSString* FNumber_key = @"FNumber";
        NSNumber* FNumber_value = [exifTree objectForKey:FNumber_key];
        type_check_obj = [exif_dict objectForKey:FNumber_key];
        if(FNumber_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            FNumber = [FNumber_value doubleValue];

            if(!QString::number(FNumber).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("FNumber").toLocal8Bit().data()),QString::number(FNumber)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }

        /// Flash
        double    Flash = 0;
        NSString* Flash_key = @"Flash";
        NSNumber* Flash_value = [exifTree objectForKey:Flash_key];
        type_check_obj = [exif_dict objectForKey:Flash_key];
        if(Flash_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            Flash = [Flash_value doubleValue];

            if(!QString::number(Flash).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("Flash").toLocal8Bit().data()),QString::number(Flash)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }

        /// FocalLenIn35mmFilm
        double    FocalLen_mm = 0;
        NSString* FocalLen_mm_key = @"FocalLenIn35mmFilm";
        NSNumber* FocalLen_mm_value = [exifTree objectForKey:FocalLen_mm_key];
        type_check_obj = [exif_dict objectForKey:FocalLen_mm_key];
        if(FocalLen_mm_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            FocalLen_mm = [FocalLen_mm_value doubleValue];

            if(!QString::number(FocalLen_mm).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("FocalLenIn35mmFilm").toLocal8Bit().data()),QString::number(FocalLen_mm)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Focal Length Details"));
            }
        }

        /// FocalLength
        double    FocalLength = 0;
        NSString* FocalLength_key = @"FocalLength";
        NSNumber* FocalLength_value = [exifTree objectForKey:FocalLength_key];
        type_check_obj = [exif_dict objectForKey:FocalLength_key];
        if(FocalLength_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            FocalLength = [FocalLength_value doubleValue];

            if(!QString::number(FocalLength).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("FocalLength").toLocal8Bit().data()),QString::number(FocalLength)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Focal Length Details"));
            }
        }


        /// LensMake
        QString   LensMake;
        NSString* LensMake_key = @"LensMake";
        NSString* LensMake_value = [exifTree objectForKey:LensMake_key];
        type_check_obj = [exif_dict objectForKey:LensMake_key];
        if(LensMake_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            LensMake = QString::fromNSString(LensMake_value);
            if(!LensMake.isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("LensMake").toLocal8Bit().data()),LensMake
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Lens Details"));

            }
        }


        /// LensModel
        QString   LensModel;
        NSString* LensModel_key = @"LensModel";
        NSString* LensModel_value = [exifTree objectForKey:LensModel_key];
        type_check_obj = [exif_dict objectForKey:LensModel_key];
        if(LensModel_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            LensModel = QString::fromNSString(LensModel_value);

            if(!LensModel.isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("LensModel").toLocal8Bit().data()),LensModel
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Lens Details"));
            }
        }

        /// MeteringMode
        double  MeteringMode = 0;
        NSString* MeteringMode_key = @"MeteringMode";
        NSNumber* MeteringMode_value = [exifTree objectForKey:MeteringMode_key];
        type_check_obj = [exif_dict objectForKey:MeteringMode_key];
        if(MeteringMode_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            MeteringMode = [MeteringMode_value doubleValue];

            if(!QString::number(MeteringMode).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("MeteringMode").toLocal8Bit().data()),QString::number(MeteringMode)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Lens Details"));
            }
        }


        /// PixelXDimension
        double  PixelXDimension = 0;
        NSString* PixelXDimension_key = @"PixelXDimension";
        NSNumber* PixelXDimension_value = [exifTree objectForKey:PixelXDimension_key];
        type_check_obj = [exif_dict objectForKey:PixelXDimension_key];
        if(PixelXDimension_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            PixelXDimension = [PixelXDimension_value doubleValue];

            if(!QString::number(PixelXDimension).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("PixelXDimension").toLocal8Bit().data()),QString::number(PixelXDimension)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Pixel Details"));

            }
        }

        /// PixelYDimension
        double  PixelYDimension = 0;
        NSString* PixelYDimension_key = @"PixelYDimension";
        NSNumber* PixelYDimension_value = [exifTree objectForKey:PixelYDimension_key];
        type_check_obj = [exif_dict objectForKey:PixelYDimension_key];
        if(PixelYDimension_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            PixelYDimension = [PixelYDimension_value doubleValue];

            if(!QString::number(PixelYDimension).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("PixelYDimension").toLocal8Bit().data()),QString::number(PixelYDimension)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Pixel Details"));
            }
        }

        /// SceneCaptureType
        double  SceneCaptureType = 0;
        NSString* SceneCaptureType_key = @"SceneCaptureType";
        NSNumber* SceneCaptureType_value = [exifTree objectForKey:SceneCaptureType_key];
        type_check_obj = [exif_dict objectForKey:SceneCaptureType_key];
        if(SceneCaptureType_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            SceneCaptureType = [SceneCaptureType_value doubleValue];

            if(!QString::number(SceneCaptureType).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("SceneCaptureType").toLocal8Bit().data()),QString::number(SceneCaptureType)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }


        /// SceneType
        double  SceneType = 0;
        NSString* SceneType_key = @"SceneType";
        NSNumber* SceneType_value = [exifTree objectForKey:SceneType_key];
        type_check_obj = [exif_dict objectForKey:SceneType_key];
        if(SceneType_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            SceneType = [SceneType_value doubleValue];

            if(!QString::number(SceneType).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("SceneType").toLocal8Bit().data()),QString::number(SceneType)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }

        /// SensingMethod
        double  SensingMethod = 0;
        NSString* SensingMethod_key = @"SensingMethod";
        NSNumber* SensingMethod_value = [exifTree objectForKey:SensingMethod_key];
        type_check_obj = [exif_dict objectForKey:SensingMethod_key];
        if(SensingMethod_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            SensingMethod = [SensingMethod_value doubleValue];

            if(!QString::number(SensingMethod).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("LensModel").toLocal8Bit().data()),QString::number(SceneType)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Lens Details"));
            }
        }



        /// ShutterSpeedValue
        double  ShutterSpeedValue = 0;
        NSString* ShutterSpeedValue_key = @"ShutterSpeedValue";
        NSNumber* ShutterSpeedValue_value = [exifTree objectForKey:ShutterSpeedValue_key];
        type_check_obj = [exif_dict objectForKey:ShutterSpeedValue_key];
        if(ShutterSpeedValue_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ShutterSpeedValue = [ShutterSpeedValue_value doubleValue];
            if(!QString::number(ShutterSpeedValue).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("ShutterSpeedValue").toLocal8Bit().data()),QString::number(ShutterSpeedValue)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));

            }
        }


        /// SubsecTimeDigitized
        double  SubsecTimeDigitized = 0;
        NSString* SubsecTimeDigitized_key = @"SubsecTimeDigitized";
        NSNumber* SubsecTimeDigitized_value = [exifTree objectForKey:SubsecTimeDigitized_key];
        type_check_obj = [exif_dict objectForKey:SubsecTimeDigitized_key];
        if(SubsecTimeDigitized_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            SubsecTimeDigitized = [SubsecTimeDigitized_value doubleValue];
            if(!QString::number(SubsecTimeDigitized).isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("SubsecTimeDigitized").toLocal8Bit().data()),QString::number(SubsecTimeDigitized)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("DateTime Details"));

            }
        }



        /// SubsecTimeOriginal
        double  SubsecTimeOriginal = 0;
        NSString* SubsecTimeOriginal_key = @"SubsecTimeOriginal";
        NSNumber* SubsecTimeOriginal_value = [exifTree objectForKey:SubsecTimeOriginal_key];
        type_check_obj = [exif_dict objectForKey:SubsecTimeOriginal_key];
        if(SubsecTimeOriginal_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            SubsecTimeOriginal = [SubsecTimeOriginal_value doubleValue];

            if(!QString::number(SubsecTimeOriginal).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("SubsecTimeOriginal").toLocal8Bit().data()),QString::number(SubsecTimeOriginal)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("DateTime Details"));
            }
        }

        /// WhiteBalance
        double  WhiteBalance = 0;
        NSString* WhiteBalance_key = @"WhiteBalance";
        NSNumber* WhiteBalance_value = [exifTree objectForKey:WhiteBalance_key];
        type_check_obj = [exif_dict objectForKey:WhiteBalance_key];
        if(WhiteBalance_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            WhiteBalance = [WhiteBalance_value doubleValue];

            if(!QString::number(WhiteBalance).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("WhiteBalance").toLocal8Bit().data()),QString::number(WhiteBalance)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));
            }
        }



        /// ************************GPS chunk******************************
        id gps_Tree = [exif_dict objectForKey:@"{GPS}"];

        QString south_or_north;

        NSString* latitude_ref_key = @"LatitudeRef";
        NSString* latitude_ref_value = [gps_Tree objectForKey:latitude_ref_key];
        type_check_obj = [gps_Tree objectForKey:latitude_ref_key];
        if(latitude_ref_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            south_or_north = QString::fromNSString(latitude_ref_value);

        }


        double  m_latitude = 0;
        NSString* latitude_key = @"Latitude";
        NSNumber* latitude_value = [gps_Tree objectForKey:latitude_key];
        type_check_obj = [gps_Tree objectForKey:latitude_key];
        if(latitude_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {

            m_latitude = [latitude_value doubleValue];

            QString lat_final = QString::number(m_latitude);
            lat_final = lat_final.trimmed();

            if(!lat_final.isEmpty())
            {
                if(south_or_north.contains("S",Qt::CaseInsensitive))
                {
                    lat_final.prepend("-");
                }


                xml_writer.writeTextElement(QObject::tr(QString("Latitude").toLocal8Bit().data()),lat_final
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("GPS"));

                exif_data_parsed_obj.exif_latitude = lat_final;


            }
        }


        QString east_or_west;

        NSString* longitude_ref_key = @"LongitudeRef";
        NSString* longitude_ref_value = [gps_Tree objectForKey:longitude_ref_key];
        type_check_obj = [gps_Tree objectForKey:longitude_ref_key];
        if(longitude_ref_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            east_or_west = QString::fromNSString(longitude_ref_value);

        }



        double  m_longitude = 0;
        NSString* longitude_key = @"Longitude";
        NSNumber* longitude_value = [gps_Tree objectForKey:longitude_key];
        type_check_obj = [gps_Tree objectForKey:longitude_key];
        if(longitude_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            m_longitude = [longitude_value doubleValue];

            QString long_final = QString::number(m_longitude);
            long_final = long_final.trimmed();


            if(!long_final.isEmpty())
            {

                if(east_or_west.contains("W", Qt::CaseInsensitive))
                {
                    long_final.prepend("-");
                }

                xml_writer.writeTextElement(QObject::tr(QString("Longitude").toLocal8Bit().data()),long_final
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("GPS"));

                exif_data_parsed_obj.exif_longitude = long_final;


            }
        }


        /// ************************TIFF chunk******************************
        id tiff_Tree = [exif_dict objectForKey:@"{TIFF}"];

        /// DateTime
        QString  DateTime;
        NSString* DateTime_key = @"DateTime";
        NSString* DateTime_value = [tiff_Tree objectForKey:DateTime_key];
        type_check_obj = [tiff_Tree objectForKey:DateTime_key];
        if(DateTime_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            DateTime = QString::fromNSString(DateTime_value);

            if(!DateTime.isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("DateTime").toLocal8Bit().data()), DateTime
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("DateTime Details"));

            }
        }


        /// Make
        QString  Make;
        NSString* Make_key = @"Make";
        NSString* Make_value = [tiff_Tree objectForKey:Make_key];
        type_check_obj = [tiff_Tree objectForKey:Make_key];
        if(Make_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            Make = QString::fromNSString(Make_value);


            if(!Make.isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("Make").toLocal8Bit().data()),Make
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                exif_data_parsed_obj.exif_make = Make;
            }
        }


        /// Model
        QString  Model;
        NSString* Model_key = @"Model";
        NSString* Model_value = [tiff_Tree objectForKey:Model_key];
        type_check_obj = [tiff_Tree objectForKey:Model_key];
        if(Model_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            Model = QString::fromNSString(Model_value);

            if(!Model.isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("Model").toLocal8Bit().data()),Model
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

                exif_data_parsed_obj.exif_model = Model;
            }
        }




        /// Orientation
        double  Orientation_tiff = 0;
        NSString* Orientation_tiff_key = @"Orientation";
        NSNumber* Orientation_tiff_value = [tiff_Tree objectForKey:Orientation_tiff_key];
        type_check_obj = [tiff_Tree objectForKey:Orientation_tiff_key];
        if(Orientation_tiff_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            Orientation_tiff = [Orientation_tiff_value doubleValue];


        }

        /// ResolutionUnit
        double  ResolutionUnit = 0;
        NSString* ResolutionUnit_key = @"ResolutionUnit";
        NSNumber* ResolutionUnit_value = [tiff_Tree objectForKey:ResolutionUnit_key];
        type_check_obj = [tiff_Tree objectForKey:ResolutionUnit_key];
        if(ResolutionUnit_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            ResolutionUnit = [ResolutionUnit_value doubleValue];

            if(!QString::number(ResolutionUnit).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("ResolutionUnit").toLocal8Bit().data()),QString::number(ResolutionUnit)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Image Settings"));

            }
        }



        /// Software
        QString  Software;
        NSString* Software_key = @"Software";
        NSString* Software_value = [tiff_Tree objectForKey:Software_key];
        type_check_obj = [tiff_Tree objectForKey:Software_key];
        if(Software_value != nil || [type_check_obj isKindOfClass:[NSString class]])
        {
            Software = QString::fromNSString(Software_value);

            if(!Software.isEmpty())
            {
                xml_writer.writeTextElement(QObject::tr(QString("Software").toLocal8Bit().data()), Software
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("File Details"));

            }
        }



        /// XResolution
        double  XResolution = 0;
        NSString* XResolution_key = @"XResolution";
        NSNumber* XResolution_value = [tiff_Tree objectForKey:XResolution_key];
        type_check_obj = [tiff_Tree objectForKey:XResolution_key];
        if(XResolution_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            XResolution = [XResolution_value doubleValue];

            if(!QString::number(XResolution).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("XResolution").toLocal8Bit().data()),QString::number(XResolution)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Resolution"));

            }
        }


        /// YResolution
        double  YResolution = 0;
        NSString* YResolution_key = @"YResolution";
        NSNumber* YResolution_value = [tiff_Tree objectForKey:YResolution_key];
        type_check_obj = [tiff_Tree objectForKey:YResolution_key];
        if(YResolution_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
        {
            YResolution = [YResolution_value doubleValue];

            if(!QString::number(YResolution).isEmpty())
            {

                xml_writer.writeTextElement(QObject::tr(QString("YResolution").toLocal8Bit().data()),QString::number(YResolution)
                                            + QString(MACRO_RECON_Splitter_1_multiple) + QString("Resolution"));
            }
        }


        xml_writer.writeEndElement();
        xml_writer.writeEndDocument();


        return QString::fromLocal8Bit(arr_output_key);

    }
    else
        return QString("");

}

void exif_data_image_files::check_metadata_keys_exist_in_nsdictionary(void *ptr_nsdictionary)
{

    NSDictionary *dict;

    dict = static_cast<NSDictionary *>(ptr_nsdictionary);

    for(id key in dict)
    {
        QString key_str = QString::fromNSString(key);


        NSObject* my_object = [dict objectForKey:key];
        if([my_object isKindOfClass:[NSDictionary class]])
        {
            NSDictionary *dicttmp = [dict objectForKey:key];

            check_metadata_keys_exist_in_nsdictionary(dicttmp);
        }

        all_keys_list << key_str;
    }

}


QStringList exif_data_image_files::extract_all_available_exif_data_image_files(QString file_path)
{ // extract image files

    ///==========================================================================///

    //
    // All the exif data is contained by exif_dict in the form of dictionary as
    //

    //    ColorModel = RGB;
    //       DPIHeight = 72;
    //       DPIWidth = 72;
    //       Depth = 8;
    //       Orientation = 6;
    //       PixelHeight = 2448;
    //       PixelWidth = 3264;
    //       ProfileName = "sRGB IEC61966-2.1";
    //       "{ExifAux}" =     {
    //           Regions =         {
    //               HeightAppliedTo = 2448;
    //               RegionList =             (
    //                                   {
    //                       AngleInfoRoll = 270;
    //                       AngleInfoYaw = 0;
    //                       ConfidenceLevel = 1000;
    //                       FaceID = 5;
    //                       Height = "0.079258";
    //                       Timestamp = 2147483647;
    //                       Type = Face;
    //                       Width = "0.059145";
    //                       X = "0.339107";
    //                       Y = "0.360044";
    //                   }
    //               );
    //               WidthAppliedTo = 3264;
    //           };
    //       };
    //       "{Exif}" =     {
    //           ApertureValue = "2.52606882168926";
    //           BrightnessValue = "4.135244793075467";
    //           ColorSpace = 1;
    //           ComponentsConfiguration =         (
    //               1,
    //               2,
    //               3,
    //               0
    //           );
    //           DateTimeDigitized = "2016:09:16 06:51:25";
    //           DateTimeOriginal = "2016:09:16 06:51:25";
    //           ExifVersion =         (
    //               2,
    //               2,
    //               1
    //           );
    //           ExposureBiasValue = 0;
    //           ExposureMode = 0;
    //           ExposureProgram = 2;
    //           ExposureTime = "0.0303030303030303";
    //           FNumber = "2.4";
    //           Flash = 32;
    //           FlashPixVersion =         (
    //               1,
    //               0
    //           );
    //           FocalLenIn35mmFilm = 31;
    //           FocalLength = "3.3";
    //           ISOSpeedRatings =         (
    //               32
    //           );
    //           LensMake = Apple;
    //           LensModel = "iPad mini 4 back camera 3.3mm f/2.4";
    //           LensSpecification =         (
    //               "3.3",
    //               "3.3",
    //               "2.4",
    //               "2.4"
    //           );
    //           MeteringMode = 5;
    //           PixelXDimension = 3264;
    //           PixelYDimension = 2448;
    //           SceneCaptureType = 0;
    //           SceneType = 1;
    //           SensingMethod = 2;
    //           ShutterSpeedValue = "5.058989898989899";
    //           SubjectArea =         (
    //               1631,
    //               1223,
    //               1795,
    //               1077
    //           );
    //           SubsecTimeDigitized = 850;
    //           SubsecTimeOriginal = 850;
    //           WhiteBalance = 0;
    //       };
    //       "{MakerApple}" =     {
    //           1 = 4;
    //           14 = 0;
    //           2 = <16001b00 27002000 5e00dc00 57006f00 16000d00 09000500 02000000 00000000 15001300 26008200 1c011a02 5f018800 22001200 0d000900 04000200 00000000 15001f00 90003300 6d006b02 0102a701 63001700 11000d00 07000300 01000000 18007800 21002000 49001602 ac017901 72001c00 13000f00 0d000600 03000000 3e006e00 1c003200 1d00be00 7a01bc00 8c00b600 93005900 2a000e00 05000200 80008500 16002600 43006300 ec00dc00 63011501 d5007a00 2d001600 08000300 86008d00 18002d00 2a008d00 1e017101 50013001 fe00c100 79003f00 11000600 8a009f00 19005100 4b00f601 bf013b01 0101d600 9c003100 2c002c00 16000800 8d00a700 38003800 53002a02 d7019f01 f2003200 e5000f00 1b002900 1b000a00 8d008800 17003c00 36002902 7d02fd01 0b013800 d4001000 1f002f00 20000e00 8d006e00 15003000 26002302 ad02eb01 0e013100 c3002100 1f003400 24001000 8d006d00 14002300 2500e601 1c02d501 0d012800 2c000b00 1b003800 29001300 8b009100 24004100 2600bf01 9401bf01 0a012400 0c000900 19003900 29001400 86009b00 57004200 30009501 8101a701 f0001f00 0a000a00 19003d00 2e001600 80007300 83007e00 34007f01 c8010201 5c001900 0a001200 24004000 31001500 58004f00 47004900 b7009001 5d015700 76001400 0a001e00 36004400 31001600>;
    //           20 = 4;
    //           3 =         {
    //               epoch = 0;
    //               flags = 1;
    //               timescale = 1000000000;
    //               value = 2613356245333;
    //           };
    //           4 = 1;
    //           5 = 143;
    //           6 = 131;
    //           7 = 1;
    //           8 =         (
    //               "0.008843093",
    //               "-0.9765947",
    //               "-0.197011"
    //           );
    //           9 = 275;
    //       };
    //       "{TIFF}" =     {
    //           DateTime = "2016:09:16 06:51:25";
    //           Make = Apple;
    //           Model = "iPad mini 4";
    //           Orientation = 6;
    //           ResolutionUnit = 2;
    //           Software = "10.0.1";
    //           XResolution = 72;
    //           YResolution = 72;
    //       };
    //   }

    ///==========================================================================///





    NSString *target_file = file_path.toNSString();


    NSDictionary* exif_dict   =   nil;
    NSURL       * url         =   [ NSURL fileURLWithPath : target_file ];

    if (url != NULL)
    {
        CGImageSourceRef source = CGImageSourceCreateWithURL ( (CFURLRef) url, NULL);

        if(source != NULL)
        {
            CFDictionaryRef metadataRef = CGImageSourceCopyPropertiesAtIndex ( source, 0, NULL );
            if ( metadataRef )
            {
                NSDictionary* immutableMetadata = (__bridge NSDictionary *)metadataRef;

                if (immutableMetadata)
                {
                    exif_dict = [NSDictionary dictionaryWithDictionary : (__bridge NSDictionary *)metadataRef];
                }

                CFRelease ( metadataRef );
            }


            CFRelease(source);
            source = nil;
        }
    }

    QStringList all_value_list;
    if (exif_dict != NULL)
    {
        id root_keys = [exif_dict allKeys];

        for(NSString *key_str in root_keys)
        {
            id root_value = [exif_dict objectForKey:key_str];

            if([root_value isKindOfClass:[NSDictionary class]])
            {
                NSDictionary *first_level_dict =  root_value;

                id first_level_keys = [first_level_dict allKeys];

                for(NSString *first_level_key_str in first_level_keys)
                {
                    id first_level_value = [first_level_dict objectForKey:first_level_key_str];

                    if([first_level_value isKindOfClass:[NSString class]])
                    {
                        all_value_list << QString::fromNSString(first_level_key_str) + QString(MACRO_RECON_Splitter_1_multiple) + QString::fromNSString(first_level_value);
                    }
                    else if([first_level_value isKindOfClass:[NSNumber class]])
                    {
                        double double_value = 0;
                        double_value = [first_level_value doubleValue];

                        all_value_list << QString::fromNSString(first_level_key_str) + QString(MACRO_RECON_Splitter_1_multiple) + QString::number(double_value);
                    }







                }
            }
            else if([root_value isKindOfClass:[NSString class]])
            {
                all_value_list << QString::fromNSString(key_str) + QString(MACRO_RECON_Splitter_1_multiple) + QString::fromNSString(root_value);
            }
            else if([root_value isKindOfClass:[NSNumber class]])
            {
                double double_value = 0;
                double_value = [root_value doubleValue];

                all_value_list << QString::fromNSString(key_str) + QString(MACRO_RECON_Splitter_1_multiple) + QString::number(double_value);
            }
        }

    }

    return all_value_list;



}
QMap<QString, QString> exif_data_image_files::pub_extract_specific_attributes(QString file_path, QStringList received_attr_list)
{ // extract exif data attributes
    NSString *target_file = file_path.toNSString();


    NSDictionary* exif_dict   =   nil;
    NSURL       * url         =   [ NSURL fileURLWithPath : target_file ];

    if (url != NULL)
    {
        CGImageSourceRef source = CGImageSourceCreateWithURL ( (CFURLRef) url, NULL);

        if(source != NULL)
        {
            CFDictionaryRef metadataRef = CGImageSourceCopyPropertiesAtIndex ( source, 0, NULL );
            if ( metadataRef )
            {
                NSDictionary* immutableMetadata = (__bridge NSDictionary *)metadataRef;

                if (immutableMetadata)
                {
                    exif_dict = [NSDictionary dictionaryWithDictionary : (__bridge NSDictionary *)metadataRef];
                }

                CFRelease ( metadataRef );
            }


            CFRelease(source);
            source = nil;
        }
    }

    id gps_Tree = [exif_dict objectForKey:@"{GPS}"];


    NSObject* type_check_obj;

    QString lat_final;
    QString south_or_north;
    NSString* latitude_ref_key = @"LatitudeRef";
    NSString* latitude_ref_value = [gps_Tree objectForKey:latitude_ref_key];
    type_check_obj = [gps_Tree objectForKey:latitude_ref_key];
    if(latitude_ref_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    {
        south_or_north = QString::fromNSString(latitude_ref_value);

    }

    double  m_latitude = 0;
    NSString* latitude_key = @"Latitude";
    NSNumber* latitude_value = [gps_Tree objectForKey:latitude_key];
    type_check_obj = [gps_Tree objectForKey:latitude_key];
    if(latitude_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    {

        m_latitude = [latitude_value doubleValue];

        lat_final = QString::number(m_latitude);
        lat_final = lat_final.trimmed();

        if(!lat_final.isEmpty())
        {
            if(south_or_north.contains("S",Qt::CaseInsensitive))
            {
                lat_final.prepend("-");
            }
        }
    }



    QString long_final;
            QString east_or_west;
    NSString* longitude_ref_key = @"LongitudeRef";
    NSString* longitude_ref_value = [gps_Tree objectForKey:longitude_ref_key];
    type_check_obj = [gps_Tree objectForKey:longitude_ref_key];
    if(longitude_ref_value != nil || [type_check_obj isKindOfClass:[NSString class]])
    {
        east_or_west = QString::fromNSString(longitude_ref_value);

    }

    double  m_longitude = 0;
    NSString* longitude_key = @"Longitude";
    NSNumber* longitude_value = [gps_Tree objectForKey:longitude_key];
    type_check_obj = [gps_Tree objectForKey:longitude_key];
    if(longitude_value != nil || [type_check_obj isKindOfClass:[NSNumber class]])
    {
        m_longitude = [longitude_value doubleValue];

        long_final = QString::number(m_longitude);
        long_final = long_final.trimmed();


        if(!long_final.isEmpty())
        {

            if(east_or_west.contains("W", Qt::CaseInsensitive))
            {
                long_final.prepend("-");
            }
        }
    }

    QMap<QString, QString> desired_map ;

    desired_map.insert("Latitude", lat_final);
    desired_map.insert("Longitude", long_final);


    //    QStringList all_key_n_value_list = extract_all_available_exif_data_image_files(filepath);

    //    for(int tt = 0; tt < received_attr_list.size(); tt++)
    //    {
    //        QString attr_name = received_attr_list.at(tt).trimmed();
    //        for(int zz = 0; zz < all_key_n_value_list.size(); zz++)
    //        {
    //            QString key_n_value = all_key_n_value_list.at(zz);
    //            if(key_n_value.contains(attr_name))
    //            {
    //                QStringList sep_list = key_n_value.split(QString(MACRO_RECON_Splitter));
    //                if(sep_list.size() < 2)
    //                    continue;

    //                QString key_name = sep_list.at(0);
    //                key_name = key_name.trimmed();
    //                if(attr_name == key_name)
    //                {
    //                    desired_map.insert(key_name, sep_list.at(1));
    //                }

    //            }

    //        }

    //    }

    return desired_map;
}

