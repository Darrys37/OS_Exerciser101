#ifndef STORAGE_H
#define STORAGE_H
#include <QString>
#include <QVector>
namespace storage {

struct DriveInfo {
    QString letter
        ;
    QString type
        ;
    double totalGB
        = 0.0
        ;
    double freeGB
        = 0.0
        ;
    double freePercent
        = 0.0
        ;
};
QVector
    <DriveInfo> getDrives();
}

#endif // STORAGE_H
