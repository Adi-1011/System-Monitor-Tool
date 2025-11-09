#include "../include/SystemMonitor.h"
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
    SystemMonitor monitor;
    // this monitor the resources and usage for upto total i updates

    for (int i = 0; i < 3; ++i) { // temporary 5 updates for test
        system("clear");
        monitor.display();
        this_thread::sleep_for(std::chrono::seconds(2));
    }


    // this monitors the resources and usage until manually terminated
    // while (true) {
    //     system("clear");
    //     monitor.display();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));  // refresh every 2s
    // }

    // implementation to kill process based on PID
    int pid;
    cout << "\n ENTER PID to kill (or 0 to skip): ";
    cin >> pid;

    if(pid > 0){
        monitor.killProcess(pid);
    }
    else {
        cout << "No process terminated.\n";
    }

    return 0;
}
