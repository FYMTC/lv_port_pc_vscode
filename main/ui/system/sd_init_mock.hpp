// Mock SD card initialization
#pragma once

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// SD卡状态
typedef enum {
    SD_CARD_STATUS_NOT_PRESENT = 0,
    SD_CARD_STATUS_PRESENT,
    SD_CARD_STATUS_MOUNTED,
    SD_CARD_STATUS_ERROR
} sd_card_status_t;

// SD卡信息
typedef struct {
    char name[32];
    uint64_t size_bytes;
    uint32_t sector_size;
    bool is_mounted;
    char mount_point[16];
} sd_card_info_t;

/**
 * @brief 初始化SD卡
 * @return ESP_OK 成功，其他值表示失败
 */
esp_err_t sd_card_init(void);

/**
 * @brief 反初始化SD卡
 */
void sd_card_deinit(void);

/**
 * @brief 获取SD卡状态
 */
sd_card_status_t sd_card_get_status(void);

/**
 * @brief 获取SD卡信息
 */
esp_err_t sd_card_get_info(sd_card_info_t* info);

/**
 * @brief 检查SD卡是否可用
 */
bool sd_card_is_available(void);

#ifdef __cplusplus
}
#endif
