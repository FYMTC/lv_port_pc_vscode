// Mock sdkconfig definitions
#pragma once

// Mock definitions for compilation
#define CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE 2048
#define CONFIG_ESP_MAIN_TASK_STACK_SIZE 4096

// Log level configuration
#define CONFIG_LOG_DEFAULT_LEVEL 3
#define CONFIG_LOG_MAXIMUM_LEVEL 5

// WiFi configuration
#define CONFIG_ESP32_WIFI_AMPDU_TX_ENABLED 1
#define CONFIG_ESP32_WIFI_AMPDU_RX_ENABLED 1

// Other mock config values
#define CONFIG_FREERTOS_HZ 1000

// Log configuration
#define CONFIG_LOG_MAXIMUM_LEVEL 5
#define CONFIG_LOG_COLORS 1
#define CONFIG_LOG_TIMESTAMP_SOURCE_RTOS 1
#define CONFIG_BOOTLOADER_LOG_LEVEL 3
#define BOOTLOADER_BUILD 0
