#include "sd_init_windows.h"
#include <stdio.h>
#include <string.h>

// Mock global variables
static bool sd_initialized = false;
static bool sd_mounted = false;

void sd_init() {
    sd_initialized = true;
    printf("SD Card: Initialized (Mock Mode)\n");
}

void mount_sd_card() {
    if (sd_initialized) {
        sd_mounted = true;
        printf("SD Card: Mounted (Mock Mode)\n");
    }
}

void unmount_sd_card() {
    sd_mounted = false;
    printf("SD Card: Unmounted (Mock Mode)\n");
}

bool is_sd_card_mounted() {
    return sd_mounted;
}

esp_err_t get_sd_card_info(sd_card_info_t *info) {
    if (!info) {
        return ESP_ERR_INVALID_ARG;
    }

    // Fill with mock data
    strncpy(info->name, "MockSD32GB", sizeof(info->name) - 1);
    info->name[sizeof(info->name) - 1] = '\0';

    info->size_bytes = 32ULL * 1024 * 1024 * 1024; // 32GB
    info->sector_size = 512;
    info->is_mounted = sd_mounted;
    strncpy(info->mount_point, "/sdcard", sizeof(info->mount_point) - 1);
    info->mount_point[sizeof(info->mount_point) - 1] = '\0';

    return ESP_OK;
}

void gpio_isr_handler(void *arg) {
    // Mock GPIO interrupt handler
    (void)arg; // Suppress unused parameter warning
}

void SD_gpio_task(void *arg) {
    // Mock GPIO task
    (void)arg; // Suppress unused parameter warning
}
