#include "../include/SystemMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

//  CPU USAGE 
double SystemMonitor::getCPUUsage() {
    static long prevIdle = 0, prevTotal = 0;
    ifstream file("/proc/stat");
    string label;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    file >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long idleAll = idle + iowait;
    long nonIdle = user + nice + system + irq + softirq + steal;
    long total = idleAll + nonIdle;

    long totalDiff = total - prevTotal;
    long idleDiff = idleAll - prevIdle;

    prevTotal = total;
    prevIdle = idleAll;

    if (totalDiff == 0) return 0;
    return (double)(totalDiff - idleDiff) / totalDiff * 100.0;
}

// MEMORY USAGE 
double SystemMonitor::getMemoryUsage(double &usedMem, double &totalMem) {
    ifstream file("/proc/meminfo");
    string key;
    long value;
    string unit;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") totalMem = value / 1024.0;
        if (key == "MemAvailable:") usedMem = totalMem - (value / 1024.0);
    }

    return (usedMem / totalMem) * 100.0;
}

//  DISPLAY 
void SystemMonitor::display() {
    double used = 0, total = 0;
    double cpu = getCPUUsage();
    double mem = getMemoryUsage(used, total);

    cout << "---------------------------------------------\n";
    cout << " CPU Usage: " << cpu << "%\n";
    cout << " Memory Usage: " << used << " MB / " << total << " MB ("
         << mem << "%)\n";
    cout << "---------------------------------------------\n";
}
