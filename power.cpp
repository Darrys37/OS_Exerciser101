#include "power.h"
#include <Windows.h>
#include <wchar.h>
#include <QString>
#include <QVector>
namespace power
{
PowerStatus getPowerStatus()
{PowerStatus ps
        ;
    SYSTEM_POWER_STATUS sps{};
    if
        (GetSystemPowerStatus
         (
             &sps))
    {
        switch
            (sps
                 .ACLineStatus
             )
        {
        case
            0
            :
            ps
                .acLineStatus
                = "Offline"
                ;
            break
                ;
        case
            1
            :
            ps
                .acLineStatus
                = "Online"
                ;
            break
                ;
        default
            :
            ps
                .acLineStatus
                = "Unknown"
                ;
            break
                ;
        }
        if
            (sps
                 .BatteryFlag == 255
             )
        {
            ps
                .batteryFlag
                = "Unknown"
                ;
        }
        else {
            QStringList flags
                ;
            if
                (sps
                     .BatteryFlag
                 &
                 1
                 )
                flags << "High"
                    ;
            if
                (sps
                     .BatteryFlag
                 &
                 2
                 )
                flags << "Low"
                    ;
            if
                (sps
                     .BatteryFlag
                 &
                 4
                 )
                flags << "Critical"
                    ;
            if
                (sps
                     .BatteryFlag
                 &
                 8
                 )
                flags << "Charging"
                    ;
            if
                (sps
                     .BatteryFlag
                 & 128
                 )
                flags << "NoBattery"
                    ;ps.batteryFlag = flags.isEmpty() ? "OK" : flags.join("|");
        }
        ps.batteryPercent = (sps.BatteryLifePercent <= 100) ? sps.BatteryLifePercent : -1;
    }
    else
    {
        ps.acLineStatus = "Unknown";
        ps.batteryFlag = "Unknown";
        ps.batteryPercent = -1;
    }
    return ps;
}
}
