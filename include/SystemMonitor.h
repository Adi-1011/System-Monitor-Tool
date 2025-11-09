#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <string>
#include <vector>
using namespace std;

struct ProcessInfo
{
    int pid;
    string name;
    char state;
    double memUsage;
    double cpuUsage;
};


class SystemMonitor {
public:
    double getCPUUsage();    // Fetch CPU usage
    double getMemoryUsage(double &usedMem, double &totalMem); // Fetch memory usage
    vector<ProcessInfo> getProcesses();  // Reads process list
    void display();     // Display formatted output
};

#endif
