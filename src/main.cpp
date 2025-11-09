#include "../include/SystemMonitor.h"
#include <thread>
#include <chrono>
#include <cstdlib>

int main() {
    SystemMonitor monitor;
    // this monitor the resources and usage for upto total i updates

    for (int i = 0; i < 8; ++i) { // temporary 5 updates for test
        system("clear");
        monitor.display();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }


    // this monitors the resources and usage until manually terminated
    // while (true) {
    //     system("clear");
    //     monitor.display();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));  // refresh every 2s
    // }
    return 0;
}
