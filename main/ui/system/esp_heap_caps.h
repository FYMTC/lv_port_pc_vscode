// Mock ESP32 heap capabilities for Windows
#pragma once

#include <cstdlib>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// Memory capability flags
#define MALLOC_CAP_SPIRAM     (1 << 0)  ///< Memory is in external SPI RAM
#define MALLOC_CAP_8BIT       (1 << 1)  ///< Memory must be 8-bit accessible
#define MALLOC_CAP_DEFAULT    0          ///< Default memory allocation

/**
 * @brief Allocate memory with specific capabilities
 * @param size Size of memory to allocate
 * @param caps Memory capabilities (ignored in mock)
 * @return Pointer to allocated memory or NULL
 */
static inline void* heap_caps_malloc(size_t size, uint32_t caps) {
    (void)caps; // Ignore capabilities in mock
    return malloc(size);
}

/**
 * @brief Free memory allocated with heap_caps_malloc
 * @param ptr Pointer to memory to free
 */
static inline void heap_caps_free(void* ptr) {
    free(ptr);
}

/**
 * @brief Get free heap size for specific capabilities
 * @param caps Memory capabilities
 * @return Free heap size in bytes
 */
static inline size_t heap_caps_get_free_size(uint32_t caps) {
    (void)caps;
    return 1024 * 1024; // Mock 1MB free
}

#ifdef __cplusplus
}
#endif
