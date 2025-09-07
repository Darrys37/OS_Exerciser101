#include "os.h"
#include <Windows.h>
#include <wchar.h>
#include <QString>
#include <QVector>
namespace Os
{
QString archToString(WORD wProcessorArchitecture)
{
    switch (wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        return "x64";
    case PROCESSOR_ARCHITECTURE_INTEL:
        return "x86";
    case PROCESSOR_ARCHITECTURE_ARM64:
        return "ARM64";
    default:
        return "Unknown";
    }
}
OsInfo getOsInfo()
{
    OsInfo o;
    typedef LONG(WINAPI * RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
    if (hMod)
    {
        auto f = reinterpret_cast<RtlGetVersionPtr>(::GetProcAddress(hMod, "RtlGetVersion"));
        if (f)
        {
            RTL_OSVERSIONINFOW vi{};
            vi.dwOSVersionInfoSize = sizeof(vi);
            if (f(&vi) == 0)
            {
                o.buildNumber = vi.dwBuildNumber;
                o.versionString = QString::number(vi.dwMajorVersion) + "." + QString::number(vi.dwMinorVersion);
            }
        }
    }
    // Product name from registry
    HKEY hKey = nullptr;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) ==
        ERROR_SUCCESS)
    {
        wchar_t buf[256];
        DWORD sz = sizeof(buf);
        if (RegQueryValueExW(hKey, L"ProductName", nullptr, nullptr, reinterpret_cast<LPBYTE>(buf), &sz) ==
            ERROR_SUCCESS)
        {
            o.productName = QString::fromWCharArray(buf);
        }
        RegCloseKey(hKey);
    }
    SYSTEM_INFO si{};
    GetNativeSystemInfo(&si);
    o.arch = archToString(si.wProcessorArchitecture);
    return o;
}
}
