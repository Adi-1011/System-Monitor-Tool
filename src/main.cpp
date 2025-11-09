#include "../include/SystemMonitor.h"
#include <thread>
#include <chrono>
#include <cstdlib>

int main() {
    SystemMonitor monitor;
    for (int i = 0; i < 5; ++i) { // temporary 5 updates for test
        system("clear");
        monitor.display();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
