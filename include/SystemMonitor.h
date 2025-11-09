#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <string>

class SystemMonitor {
public:
    double getCPUUsage();    // Fetch CPU usage
    double getMemoryUsage(double &usedMem, double &totalMem); // Fetch memory usage
    void display();     // Display formatted output
};

#endif
