#include "../include/SystemMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <csignal> // for kill()

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
#include <filesystem>
#include <algorithm>
#include <unistd.h>     // for sysconf
using namespace std;

// ---------- PROCESS LIST ----------
vector<ProcessInfo> SystemMonitor::getProcesses() {
    vector<ProcessInfo> processes;
    long ticksPerSec = sysconf(_SC_CLK_TCK);
    double uptime = 0;

    // --- Read system uptime ---
    ifstream up("/proc/uptime");
    if (up) up >> uptime;

    for (const auto &entry : filesystem::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        string dir = entry.path().filename();
        if (!all_of(dir.begin(), dir.end(), ::isdigit)) continue;

        ProcessInfo p{};
        p.pid = stoi(dir);

        // --- Read /proc/[pid]/stat ---
        ifstream statFile("/proc/" + dir + "/stat");
        if (!statFile) continue;
        string comm;
        statFile >> p.pid >> comm >> p.state;
        p.name = comm.substr(1, comm.size() - 2);

        long utime, stime, cutime, cstime, starttime;
        for (int i = 0; i < 10; ++i) statFile >> utime;  // skip 10 fields
        statFile >> utime >> stime >> cutime >> cstime >> starttime;

        // total process time in clock ticks
        double totalTime = utime + stime + cutime + cstime;
        double seconds = uptime - (starttime / (double)ticksPerSec);
        p.cpuUsage = seconds > 0 ? 100.0 * ((totalTime / ticksPerSec) / seconds) : 0.0;

        // --- Read /proc/[pid]/status for memory ---
        ifstream statusFile("/proc/" + dir + "/status");
        string key;
        long mem = 0;
        while (statusFile >> key) {
            if (key == "VmRSS:") {
                statusFile >> mem;
                p.memUsage = mem / 1024.0;
                break;
            }
        }
        processes.push_back(p);
    }

    // --- Sort by CPU usage descending ---
    sort(processes.begin(), processes.end(),
         [](const ProcessInfo &a, const ProcessInfo &b) {
             return a.cpuUsage > b.cpuUsage;
         });

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
    cout << "PID\tSTATE\tCPU(%)\tMEM(MB)\tNAME\n";

    auto processes = getProcesses();
    int count = 0;
    for (auto &p : processes) {
        if (count++ >= 15) break; // show top 15
        cout << p.pid << '\t' << p.state << '\t'
             << p.cpuUsage << '\t'
             << p.memUsage << '\t'
             << p.name << '\n';
    }
}

bool SystemMonitor::killProcess(int pid){
    if(kill(pid, SIGTERM) == 0) {
        cout << "process " << pid << " terminated successfully.\n";
        return true;
    }
    else{
        perror("Error terminating the process");
        return false;
    }
}


