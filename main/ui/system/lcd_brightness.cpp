
#include "lcd_brightness.hpp"
#include <iostream>

static const char *TAG = "brightness";

// 模拟的全局状态变量
static brightness_mode_t current_mode = BRIGHTNESS_MODE_MANUAL;
static uint8_t current_brightness = 128; // 50% 亮度
static bool auto_sleep_enabled = true;
static uint32_t sleep_timeout_ms = 30000; // 30秒

// 模拟的亮度配置
static brightness_config_t config = {
    .mode = BRIGHTNESS_MODE_MANUAL,
    .manual_brightness = 128,
    .min_brightness = 12,
    .max_brightness = 255,
    .light_threshold_low = 100,
    .light_threshold_high = 3000,
    .auto_adjust_interval = 200,
    .smooth_transition = true,
    .auto_sleep_enabled = true,
    .sleep_timeout_ms = 30000,
    .sleep_delay_ms = 3000,
    .sleep_brightness = 1
};

void create_brightness_task(void)
{
    std::cout << "Mock: Brightness task created" << std::endl;
}

void set_brightness_mode(brightness_mode_t mode)
{
    current_mode = mode;
    config.mode = mode;
    std::cout << "Mock: Brightness mode set to " << (mode == BRIGHTNESS_MODE_AUTO ? "AUTO" : "MANUAL") << std::endl;
}

brightness_mode_t get_brightness_mode(void)
{
    return current_mode;
}

void set_brightness_manual(uint8_t brightness)
{
    current_brightness = brightness;
    config.manual_brightness = brightness;
    current_mode = BRIGHTNESS_MODE_MANUAL;
    config.mode = BRIGHTNESS_MODE_MANUAL;
    std::cout << "Mock: Manual brightness set to " << (int)brightness << std::endl;
}

uint8_t get_current_brightness(void)
{
    return current_brightness;
}

void set_brightness_config(const brightness_config_t* new_config)
{
    if (new_config) {
        config = *new_config;
        std::cout << "Mock: Brightness config updated" << std::endl;
    }
}

brightness_config_t get_brightness_config(void)
{
    return config;
}

void set_brightness_status_callback(brightness_status_callback_t callback)
{
    std::cout << "Mock: Brightness status callback set" << std::endl;
}

void brightness_wake_up(void)
{
    std::cout << "Mock: Screen wake up" << std::endl;
}

void brightness_set_auto_sleep_enabled(bool enabled)
{
    auto_sleep_enabled = enabled;
    config.auto_sleep_enabled = enabled;
    std::cout << "Mock: Auto sleep " << (enabled ? "enabled" : "disabled") << std::endl;
}

bool brightness_get_auto_sleep_enabled(void)
{
    return auto_sleep_enabled;
}

void brightness_set_sleep_timeout(uint32_t timeout_ms)
{
    sleep_timeout_ms = timeout_ms;
    config.sleep_timeout_ms = timeout_ms;
    std::cout << "Mock: Sleep timeout set to " << timeout_ms << "ms" << std::endl;
}

uint32_t brightness_get_sleep_timeout(void)
{
    return sleep_timeout_ms;
}

bool brightness_is_screen_sleeping(void)
{
    return false; // 模拟屏幕始终处于唤醒状态
}

bool brightness_is_in_sleep_transition(void)
{
    return false; // 模拟不在睡眠转换过程中
}

int get_ambient_light_raw(void)
{
    return 1500; // 模拟中等光线条件
}

int get_max_brightness_range(void)
{
    return 255; // 模拟最大亮度范围
}

void reset_brightness_range(void)
{
    std::cout << "Mock: Brightness range reset" << std::endl;
}

esp_err_t brightness_save_config_to_nvs(void)
{
    std::cout << "Mock: Brightness config saved to NVS" << std::endl;
    return ESP_OK;
}

esp_err_t brightness_load_config_from_nvs(void)
{
    std::cout << "Mock: Brightness config loaded from NVS" << std::endl;
    return ESP_OK;
}

void brightness_reset_max_history(void)
{
    std::cout << "Mock: Brightness max history reset" << std::endl;
}
