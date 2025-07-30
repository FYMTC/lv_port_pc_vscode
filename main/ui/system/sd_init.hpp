// Mock SD init for PC compilation
#pragma once

#include <cstdint>

// 模拟ESP32类型定义
typedef void* TaskHandle_t;
typedef void* QueueHandle_t;
typedef struct {
    char name[32];
    uint32_t capacity;
} sdmmc_card_t;

// 模拟esp_err_t类型
typedef int esp_err_t;
#define ESP_OK 0

// GPIO和其他定义
#define GPIO_NUM_4 4
#define GPIO_NUM_5 5
#define GPIO_NUM_10 10
#define GPIO_NUM_16 16
#define GPIO_NUM_6 6
#define GPIO_NUM_2 2
#define SPI2_HOST 1
#define IRAM_ATTR

#define SDMMC_DATA2_GPIO GPIO_NUM_4
#define SDMMC_DATA3_GPIO GPIO_NUM_5
#define SD_DET_PIN GPIO_NUM_10

// SPI 配置引脚
#define SD_MISO_PIN GPIO_NUM_16
#define SD_MOSI_PIN GPIO_NUM_6
#define SD_CLK_PIN GPIO_NUM_2
#define SD_CS_PIN GPIO_NUM_5
#define SDCARD_SPIHOST SPI2_HOST

// 挂载点
#define sdcard_mount_point "/:"

// SD卡信息结构体
typedef struct {
    char name[32];
    uint64_t size_bytes;
    uint32_t sector_size;
    bool is_mounted;
    char mount_point[16];
} sd_card_info_t;

// 外部变量声明
extern TaskHandle_t GPIOtask_handle;
extern QueueHandle_t gpio_evt_queue;
extern sdmmc_card_t *card;

// 函数声明
void sd_init();
void mount_sd_card();
void unmount_sd_card();
bool is_sd_card_mounted();
esp_err_t get_sd_card_info(sd_card_info_t *info);

// 内部任务和中断处理函数声明
void gpio_isr_handler(void *arg);
void SD_gpio_task(void *arg);
