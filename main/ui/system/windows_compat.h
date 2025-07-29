// Windows compatibility functions for ESP32 project
#pragma once

#include <ctime>
#include <cmath>
#include <cstdint>  // For uint64_t

// Prevent Windows header conflicts
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32
#include <winsock2.h>  // Include this before windows.h to prevent timeval conflicts
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Define M_PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Windows compatibility for localtime_r
#ifdef _WIN32
// Check if localtime_r is already available
#ifndef __MINGW32__
static inline struct tm* localtime_r(const time_t* timep, struct tm* result) {
#ifdef _MSC_VER
    // Use MSVC's localtime_s
    if (localtime_s(result, timep) == 0) {
        return result;
    }
    return NULL;
#else
    // For other Windows compilers, use thread-safe localtime
    struct tm* temp = localtime(timep);
    if (temp) {
        *result = *temp;
        return result;
    }
    return NULL;
#endif
}
#endif
#endif

// Mock esp_restart function
void esp_restart(void);

#ifdef __cplusplus
}
#endif
