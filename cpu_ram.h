#ifndef CPU_RAM_H
#define CPU_RAM_H

#include <QString>

namespace cpu_ram {

// Thông tin CPU
struct SysInfo {
    QString arch;                     // Kiến trúc CPU: x86, x64, ARM64
    unsigned int numberOfProcessors = 0; // Số lượng CPU logical
};

// Thông tin RAM
struct MemStatus {
    int memoryLoad = 0;        // % bộ nhớ đang sử dụng
    double totalPhysGB = 0.0;  // Tổng RAM (GB)
    double availPhysGB = 0.0;  // RAM khả dụng (GB)
};

// Hàm lấy thông tin CPU
SysInfo getSystemInfo();

// Hàm lấy thông tin RAM
MemStatus getMemoryStatus();

} // namespace cpu_ram

#endif // CPU_RAM_H
