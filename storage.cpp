#include "storage.h"
#include <Windows.h>
#include <wchar.h>
#include <QString>
#include <QVector>
namespace storage {
QString driveTypeToString
    (UINT
         t
     )
{
    switch
        (
            t
            )
    {
    case DRIVE_UNKNOWN
        :
        return "Unknown"
            ;
    case DRIVE_NO_ROOT_DIR
        :
        return "NoRoot"
            ;
    case DRIVE_REMOVABLE
        :
        return "Removable"
            ;
    case DRIVE_FIXED
        :
        return "Fixed"
            ;
    case DRIVE_REMOTE
        :
        return "Remote"
            ;
    case DRIVE_CDROM
        :
        return "CDROM"
            ;
    case DRIVE_RAMDISK
        :
        return "RamDisk"
            ;
    default
        :
        return "Unknown"
            ;
    }
}
QVector
    <DriveInfo> getDrives()
{
    QVector
        <DriveInfo> out
        ;
    wchar_t buf
        [512]{};
    DWORD len
        = GetLogicalDriveStringsW
        (511, buf);
    if
        (len ==
         0
         )
        return out
            ;
    const wchar_t
        *
            p
        = buf
        ;
    while
        (
            *
            p
            )
    {
        QString root
            = QString::fromWCharArray
            (
                p);
        UINT t = GetDriveTypeW(p);
        ULARGE_INTEGER freeBytesAvailableToCaller{}, totalNumberOfBytes{}, totalNumberOfFreeBytes{};
        double totalGB = 0.0, freeGB = 0.0, freePct = 0.0;
        if (GetDiskFreeSpaceExW(p, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            totalGB = static_cast<double>(totalNumberOfBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
            freeGB = static_cast<double>(totalNumberOfFreeBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
            freePct = (totalGB > 0.0) ? (freeGB * 100.0 / totalGB) : 0.0;
        }
        out.push_back(DriveInfo{root, driveTypeToString(t), totalGB, freeGB, freePct});
        p += wcslen(p) + 1; // next null-terminated string
    }
    return out;
}
}
