#ifndef SD_INIT_WINDOWS_H
#define SD_INIT_WINDOWS_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Windows compatibility for ESP32 types
typedef void* TaskHandle_t;
typedef void* QueueHandle_t;
typedef struct {
    char name[32];
    uint32_t capacity;
    uint32_t sector_size;
} sdmmc_card_t;

// 移除IRAM_ATTR宏定义 (Windows不需要)
#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

// SD卡信息结构体
typedef struct {
    char name[32];
    uint64_t size_bytes;
    uint32_t sector_size;
    bool is_mounted;
    char mount_point[16];
} sd_card_info_t;

// 函数声明
void sd_init();
void mount_sd_card();
void unmount_sd_card();
bool is_sd_card_mounted();
esp_err_t get_sd_card_info(sd_card_info_t *info);

// Mock implementations for Windows
void gpio_isr_handler(void *arg);
void SD_gpio_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif // SD_INIT_WINDOWS_H
