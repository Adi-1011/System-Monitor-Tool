#include "../include/SystemMonitor.h"
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
    SystemMonitor monitor;
    int refreshRate = 2;
    // this monitor the resources and usage for upto total i updates
    while (true){
    for (int i = 0; i < 3; ++i) { // temporary 5 updates for test
        system("clear");
        monitor.display();
        std::this_thread::sleep_for(std::chrono::seconds(refreshRate));
    }


    // this monitors the resources and usage until manually terminated
    // while (true) {
    //     system("clear");
    //     monitor.display();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));  // refresh every 2s
    // }

    // implementation to kill process based on PID
    int pid;
    cout << "\n ENTER PID to kill (or -1 to skip): ";
    cin >> pid;

    if(pid == -1 ){
        cout << "No process terminated.\n";
        
    }
    else if (pid > 0) {
        monitor.killProcess(pid);
        cout << "Returning to live preview..\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

   
}
 return 0;
}
