// Windows compatible sys/time.h
#pragma once

#include <ctime>

#ifdef _WIN32
// Include winsock2.h to get the system timeval definition
#include <winsock2.h>

// Mock gettimeofday for Windows (use system timeval)
static inline int gettimeofday(struct timeval* tv, void* tz) {
    if (tv) {
        time_t now = time(nullptr);
        tv->tv_sec = static_cast<long>(now);
        tv->tv_usec = 0;
    }
    return 0;
}
#else
#include <sys/time.h>
#endif
