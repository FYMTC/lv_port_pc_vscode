// Windows compatibility functions implementation
#include "windows_compat.h"
#include "esp_sleep.h"  // Include for sleep function declarations
#include <cstdlib>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

// Mock implementation of esp_restart
void esp_restart(void) {
    // In a real ESP32, this would restart the device
    // For Windows simulation, we just exit the program
    std::cout << "ESP32 restart requested - exiting simulation" << std::endl;
    exit(0);
}

// Mock implementation of esp_deep_sleep_start
void esp_deep_sleep_start(void) {
    // In a real ESP32, this would enter deep sleep
    // For Windows simulation, we just exit the program
    std::cout << "ESP32 deep sleep requested - exiting simulation" << std::endl;
    exit(0);
}

// Mock implementation of esp_deep_sleep
void esp_deep_sleep(uint64_t time_in_us) {
    // In a real ESP32, this would enter deep sleep for specified time
    // For Windows simulation, we just exit the program
    std::cout << "ESP32 deep sleep for " << time_in_us << "us requested - exiting simulation" << std::endl;
    exit(0);
}

#ifdef __cplusplus
}
#endif
