#ifndef POWER_H
#define POWER_H
#include <QString>
#include <QVector>
namespace power
{
struct PowerStatus
{
    QString acLineStatus; // Online/Offline/Unknown
    QString batteryFlag; // High/Low/Critical/Charging/No battery/Unknown
    int batteryPercent = -1; // -1 unknown
};
PowerStatus getPowerStatus();
}
#endif // WINHELPERS_H
