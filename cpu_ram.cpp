#include "cpu_ram.h"
#include <Windows.h>
#include <wchar.h>
#include <QString>
#include <QVector>
namespace cpu_ram {
QString archToString
    (WORD wProcessorArchitecture
     )
{
    switch
        (wProcessorArchitecture
         )
    {
    case PROCESSOR_ARCHITECTURE_AMD64
        :
        return "x64"
            ;
    case PROCESSOR_ARCHITECTURE_INTEL
        :
        return "x86"
            ;
    case PROCESSOR_ARCHITECTURE_ARM64
        :
        return "ARM64"
            ;
    default
        :
        return "Unknown"
            ;
    }
}
SysInfo getSystemInfo() {
    SysInfo
        s
        ;
    SYSTEM_INFO si{};
    GetNativeSystemInfo
        (
            &si);
    s
        .arch
        = archToString
        (si
             .wProcessorArchitecture);
    s
        .numberOfProcessors
        = si
              .dwNumberOfProcessors
        ;
    return
        s
        ;
}
MemStatus getMemoryStatus() {
    MemStatus
        m
        ;
    MEMORYSTATUSEX ms{};
    ms
        .dwLength
        = sizeof
        (ms);
    if (GlobalMemoryStatusEx(&ms))
    {
        m.memoryLoad = static_cast<int>(ms.dwMemoryLoad);
        m.totalPhysGB = static_cast<double>(ms.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
        m.availPhysGB = static_cast<double>(ms.ullAvailPhys) / (1024.0 * 1024.0 * 1024.0);
    }
    return m;
}
}
