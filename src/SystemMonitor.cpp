#include "../include/SystemMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <filesystem>

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

#include <filesystem>
using namespace std;

// ---------- PROCESS LIST ----------
vector<ProcessInfo> SystemMonitor::getProcesses() {
    vector<ProcessInfo> processes;

    for (const auto &entry : filesystem::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        string dir = entry.path().filename();
        if (!all_of(dir.begin(), dir.end(), ::isdigit)) continue;  // only numeric dirs

        ProcessInfo p{};
        p.pid = stoi(dir);

        // --- Read /proc/[pid]/stat ---
        ifstream statFile("/proc/" + dir + "/stat");
        if (!statFile) continue;
        string comm;
        statFile >> p.pid >> comm >> p.state;
        p.name = comm.substr(1, comm.size() - 2);  // remove parentheses

        // --- Read /proc/[pid]/status for memory ---
        ifstream statusFile("/proc/" + dir + "/status");
        string key;
        long mem = 0;
        while (statusFile >> key) {
            if (key == "VmRSS:") {
                statusFile >> mem;
                p.memUsage = mem / 1024.0;  // kB→MB
                break;
            }
        }

        processes.push_back(p);
    }
    return processes;
}


//  DISPLAY 
void SystemMonitor::display() {
    double used = 0, total = 0;
    double cpu = getCPUUsage();
    double mem = getMemoryUsage(used, total);

    cout << "---------------------------------------------\n";
    cout << " CPU Usage: " << cpu << "% | Memory: " << used << " MB / "
         << total << " MB (" << mem << "%)\n";
    cout << "---------------------------------------------\n";
    cout << "PID\tSTATE\tMEM(MB)\tNAME\n";

    for (auto &p : getProcesses()) {
        cout << p.pid << '\t' << p.state << '\t'
             << p.memUsage << '\t' << p.name << '\n';
    }
}

