#include "connected_drives.h"

connected_drives::connected_drives()
{

}

void connected_drives::pub_extract_device_list() {}
//{

//    physical_device_list.clear();

//    for(int i = 0 ; i < 64; i++)
//    {
//        QString sstr = "\\\\.\\PhysicalDrive" + QString::number(i);

//        physical_device device = list_physical_device(sstr);

//        if(device.size == -1)
//            continue;

//        physical_device_list.append(device);
//    }

//    for(int i = 0 ; i < 64; i++)
//        for (int n = 0; n <= 32; n++)
//        {

//            QString sstr = "\\Device\\Harddisk" + QString::number(i) + "\\Partition" + QString::number(n);

//            physical_device device = list_physical_device(sstr);

//            if(device.size == -1)
//                continue;

//            physical_device_list.append(device);
//        }


//    QFileInfoList list =  QDir::drives();

//    foreach (QFileInfo device, list) {

//        physical_device d1 = list_logical_device(device.absoluteFilePath());

//        if(d1.size == -1)
//            continue;

//        physical_device_list.append(d1);
//    }
//}

QList<physical_device> connected_drives::pub_get_device_list()
{
    return  physical_device_list;
}


physical_device connected_drives::list_physical_device(QString sstr) {}
//{

//    physical_device device;

//    // ====
//    HANDLE dev;
//    DWORD dwResult;
//    BOOL bResult;
//    PARTITION_INFORMATION diskInfo;
//    DISK_GEOMETRY driveInfo;
//    wchar_t szDosDevice[MAX_PATH], szCFDevice[MAX_PATH];
//    static LONGLONG deviceSize = 0;
//    wchar_t size[100] = {0}, partTypeStr[1024] = {0}, *partType = partTypeStr;

//    //MultiByteToWideChar(CP_UTF8, 0, szTmp, -1 , szCFDevice, 0);
//    BOOL drivePresent = FALSE;
//    BOOL removable = FALSE;

//    drivePresent = TRUE;

//    memset(szCFDevice, 0, 260);



//    sstr.toWCharArray(szCFDevice);
//    // dev = CreateFile (L"\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
//    dev = CreateFile (szCFDevice, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

//    bResult = DeviceIoControl (dev, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0,
//                               &diskInfo, sizeof (diskInfo), &dwResult, NULL);

//    if (/* n == 0 && */ DeviceIoControl (dev, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0,
//                                         &driveInfo, sizeof (driveInfo), &dwResult, NULL))
//        removable = driveInfo.MediaType == RemovableMedia;

//    CloseHandle(dev);

//    if (!bResult)
//    {
//        device.size = -1;

//        return device;
//    }

//    switch(diskInfo.PartitionType) {
//    case PARTITION_ENTRY_UNUSED:	partType = L""; break;
//    case PARTITION_XINT13_EXTENDED:
//    case PARTITION_EXTENDED:		partType = L"Extended"; break;
//    case PARTITION_HUGE:			wsprintfW (partTypeStr, L"%s (0x%02X)", L"Unformatted", diskInfo.PartitionType); partType = partTypeStr; break;
//    case PARTITION_FAT_12:			partType = L"FAT12"; break;
//    case PARTITION_FAT_16:			partType = L"FAT16"; break;
//    case PARTITION_FAT32:
//    case PARTITION_FAT32_XINT13:	partType = L"FAT32"; break;
//    case 0x08:						partType = L"DELL (spanning)"; break;
//    case 0x12:						partType = L"Config/diagnostics"; break;
//    case 0x11:
//    case 0x14:
//    case 0x16:
//    case 0x1b:
//    case 0x1c:
//    case 0x1e:						partType = L"Hidden FAT"; break;
//    case PARTITION_IFS:				partType = L"NTFS"; break;
//    case 0x17:						partType = L"Hidden NTFS"; break;
//    case 0x3c:						partType = L"PMagic recovery"; break;
//    case 0x3d:						partType = L"Hidden NetWare"; break;
//    case 0x41:						partType = L"Linux/MINIX"; break;
//    case 0x42:						partType = L"SFS/LDM/Linux Swap"; break;
//    case 0x51:
//    case 0x64:
//    case 0x65:
//    case 0x66:
//    case 0x67:
//    case 0x68:
//    case 0x69:						partType = L"Novell"; break;
//    case 0x55:						partType = L"EZ-Drive"; break;
//    case PARTITION_OS2BOOTMGR:		partType = L"OS/2 BM"; break;
//    case PARTITION_XENIX_1:
//    case PARTITION_XENIX_2:			partType = L"Xenix"; break;
//    case PARTITION_UNIX:			partType = L"UNIX"; break;
//    case 0x74:						partType = L"Scramdisk"; break;
//    case 0x78:						partType = L"XOSL FS"; break;
//    case 0x80:
//    case 0x81:						partType = L"MINIX"; break;
//    case 0x82:						partType = L"Linux Swap"; break;
//    case 0x43:
//    case 0x83:						partType = L"Linux"; break;
//    case 0xc2:
//    case 0x93:						partType = L"Hidden Linux"; break;
//    case 0x86:
//    case 0x87:						partType = L"NTFS volume set"; break;
//    case 0x9f:						partType = L"BSD/OS"; break;
//    case 0xa0:
//    case 0xa1:						partType = L"Hibernation"; break;
//    case 0xa5:						partType = L"BSD"; break;
//    case 0xa8:						partType = L"Mac OS-X"; break;
//    case 0xa9:						partType = L"NetBSD"; break;
//    case 0xab:						partType = L"Mac OS-X Boot"; break;
//    case 0xb8:						partType = L"BSDI BSD/386 swap"; break;
//    case 0xc3:						partType = L"Hidden Linux swap"; break;
//    case 0xfb:						partType = L"VMware"; break;
//    case 0xfc:						partType = L"VMware swap"; break;
//    case 0xfd:						partType = L"Linux RAID"; break;
//    case 0xfe:						partType = L"WinNT hidden"; break;
//    default:						wsprintfW(partTypeStr, L"0x%02X", diskInfo.PartitionType); partType = partTypeStr; break;
//    }

//    // GetSizeString(diskInfo.PartitionLength.QuadPart, size);

//    device.physical_node = sstr;
//    device.size = diskInfo.PartitionLength.QuadPart;


//    return device;
//    //	char *s_type = removable ? "Removable" : "Fixed";
//    //	printf(format_str, szTmp, size, s_type, partType);

//}

physical_device connected_drives::list_logical_device(QString sstr) {}
//{
//    physical_device device;
//    device.size = -1;

//    DWORD dwVolSerial;
//    BOOL bIsRetrieved;

//    wchar_t volume_name[255];
//    wchar_t file_system_name_1[255];
//    memset(volume_name, 0, 400);
//    memset(file_system_name_1, 0, 400);

//    //const WCHAR *pwcsName = L"D:\\";

//    //   WINBASEAPI WINBOOL WINAPI GetVolumeInformationW (LPCWSTR lpRootPathName, LPWSTR lpVolumeNameBuffer, DWORD nVolumeNameSize, LPDWORD lpVolumeSerialNumber, LPDWORD lpMaximumComponentLength, LPDWORD lpFileSystemFlags, LPWSTR lpFileSystemNameBuffer, DWORD nFileSystemNameSize);

//    WCHAR pwcsName[200];
//    memset(pwcsName, 0, 200);

//    sstr.toWCharArray(pwcsName);

//    bIsRetrieved = GetVolumeInformation(pwcsName, volume_name, 200, &dwVolSerial, NULL, NULL, file_system_name_1, 200);

//    //    if (bIsRetrieved) {
//    //        printf("Serial number of drive C is %X \n",dwVolSerial);
//    //    } else {
//    //        printf("Could not retrieve\n");
//    //    }


//    BOOL  fResult;
//    unsigned __int64 i64FreeBytesToCaller,
//            i64TotalBytes,
//            i64FreeBytes;
//    fResult = GetDiskFreeSpaceEx (pwcsName,
//                                  (PULARGE_INTEGER)&i64FreeBytesToCaller,
//                                  (PULARGE_INTEGER)&i64TotalBytes,
//                                  (PULARGE_INTEGER)&i64FreeBytes);
//    if (fResult)
//    {
//        device.physical_node = sstr;
//        device.size = i64TotalBytes;
//    }

//    return device;
//}
