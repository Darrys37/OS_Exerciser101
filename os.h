#ifndef OS_H
#define OS_H
#include <QString>
#include <QVector>
namespace Os {struct OsInfo {
    QString productName; // e.g., Windows 11 Pro
    QString versionString; // e.g., 23H2 (if available)
    unsigned long buildNumber = 0; // e.g., 22631
    QString arch; // x86/x64/ARM64
};
struct SysInfo {
    QString arch;
    unsigned int numberOfProcessors = 0;
};
OsInfo getOsInfo();
}
#endif // WINHELPERS_H
