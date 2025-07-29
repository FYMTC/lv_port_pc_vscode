// Mock ESP ROM system functions
#pragma once

#include <cstdio>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif

// Mock ROM functions
static inline int esp_rom_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = vprintf(fmt, args);
    va_end(args);
    return result;
}

static inline void esp_rom_delay_us(uint32_t us) {
    // Mock delay - do nothing or use platform-specific delay
    (void)us;
}

#ifdef __cplusplus
}
#endif
