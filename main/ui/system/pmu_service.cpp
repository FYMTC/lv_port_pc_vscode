#include "pmu_service.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <cstring>

// 全局状态变量
static bool pmu_initialized = false;
static bool pmu_running = false;
static pmu_status_t current_pmu_status = PMU_STATUS_DISCONNECTED;

// 模拟数据结构
static pmu_data_t current_pmu_data;

// 回调函数指针
static pmu_data_callback_t data_callback = nullptr;
static pmu_event_callback_t event_callback = nullptr;

// 初始化模拟数据
static void init_mock_data() {
    current_pmu_data.battery_voltage = 3800;
    current_pmu_data.battery_current = -150;
    current_pmu_data.battery_percentage = 75;
    current_pmu_data.battery_status = BATTERY_STATUS_DISCHARGING;

    current_pmu_data.vbus_voltage = 0;
    current_pmu_data.vbus_current = 0;
    current_pmu_data.vbus_present = false;

    current_pmu_data.system_voltage = 3300;
    current_pmu_data.temperature = 250; // 25.0°C * 10

    // 电源通道状态
    current_pmu_data.dc1_enabled = true;
    current_pmu_data.dc3_enabled = true;
    current_pmu_data.aldo1_enabled = false;
    current_pmu_data.aldo2_enabled = false;
    current_pmu_data.aldo3_enabled = false;
    current_pmu_data.aldo4_enabled = false;
    current_pmu_data.bldo1_enabled = true;
    current_pmu_data.bldo2_enabled = false;

    current_pmu_data.charge_status = CHARGE_STATUS_NOT_CHARGING;
    current_pmu_data.charge_current = 500;
    current_pmu_data.charge_voltage = 4200;

    current_pmu_data.pmu_status = PMU_STATUS_CONNECTED;
    current_pmu_data.timestamp = 0;
}

// 更新模拟数据
static void update_mock_data() {
    static int counter = 0;
    counter++;

    // 每10次更新改变一次USB连接状态 (模拟插拔)
    if ((counter % 10) == 0) {
        if ((rand() % 100) < 10) { // 10%概率改变状态
            current_pmu_data.vbus_present = !current_pmu_data.vbus_present;

            if (current_pmu_data.vbus_present) {
                current_pmu_data.vbus_voltage = 5000 + (rand() % 200 - 100); // 4.9V-5.1V
                current_pmu_data.vbus_current = 500 + (rand() % 300);        // 500-800mA
                current_pmu_data.battery_status = (current_pmu_data.battery_percentage >= 100) ?
                                                 BATTERY_STATUS_FULL : BATTERY_STATUS_CHARGING;
                current_pmu_data.charge_status = CHARGE_STATUS_CONSTANT_CURRENT;
                current_pmu_data.battery_current = 200 + (rand() % 300); // 200-500mA充电
            } else {
                current_pmu_data.vbus_voltage = 0;
                current_pmu_data.vbus_current = 0;
                current_pmu_data.battery_status = BATTERY_STATUS_DISCHARGING;
                current_pmu_data.charge_status = CHARGE_STATUS_NOT_CHARGING;
                current_pmu_data.battery_current = -(50 + (rand() % 200)); // -50 to -250mA放电
            }

            // 触发事件回调
            if (event_callback) {
                event_callback(current_pmu_data.vbus_present ? "usb_connected" : "usb_disconnected",
                              current_pmu_data.vbus_present ? 1 : 0);
            }
        }
    }

    // 电池电量缓慢变化
    static int last_percentage = current_pmu_data.battery_percentage;
    if ((counter % 20) == 0) { // 每20次更新改变一次电量
        int change = (rand() % 3) - 1; // -1, 0, 1
        current_pmu_data.battery_percentage = std::max(0, std::min(100, last_percentage + change));
        last_percentage = current_pmu_data.battery_percentage;
    }

    // 根据电量调整电压
    current_pmu_data.battery_voltage = 3300 + (current_pmu_data.battery_percentage * 7); // 3.3V-4.0V

    // 系统电压稍有变化
    current_pmu_data.system_voltage = 3300 + (rand() % 100 - 50); // 3.25V-3.35V

    // 温度随机变化
    current_pmu_data.temperature = 250 + (rand() % 100 - 50); // 20-30°C

    // 更新时间戳
    current_pmu_data.timestamp = (uint32_t)time(nullptr);
}

esp_err_t pmu_service_init(void) {
    if (pmu_initialized) {
        return ESP_OK;
    }

    pmu_initialized = true;
    current_pmu_status = PMU_STATUS_CONNECTED;
    srand((unsigned int)time(nullptr));

    init_mock_data();

    printf("PMU Service: Initialized (Mock Mode)\n");
    return ESP_OK;
}

esp_err_t pmu_service_start(uint32_t interval_ms) {
    if (!pmu_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    pmu_running = true;
    printf("PMU Service: Started with interval %lu ms\n", (unsigned long)interval_ms);

    // 在实际实现中，这里会启动一个定时器来定期更新数据
    // 在模拟环境中，数据会在每次调用get_data时更新

    return ESP_OK;
}

esp_err_t pmu_service_stop(void) {
    pmu_running = false;
    printf("PMU Service: Stopped\n");
    return ESP_OK;
}

esp_err_t pmu_service_register_data_callback(pmu_data_callback_t callback) {
    data_callback = callback;
    return ESP_OK;
}

esp_err_t pmu_service_register_event_callback(pmu_event_callback_t callback) {
    event_callback = callback;
    return ESP_OK;
}

esp_err_t pmu_service_get_data(pmu_data_t *data) {
    if (!pmu_initialized || !data) {
        return ESP_ERR_INVALID_STATE;
    }

    // 更新模拟数据
    update_mock_data();

    // 复制数据
    *data = current_pmu_data;

    // 触发数据回调
    if (data_callback) {
        data_callback(data);
    }

    return ESP_OK;
}

pmu_status_t pmu_service_get_status(void) {
    return current_pmu_status;
}

esp_err_t pmu_service_set_charge_current(uint16_t current_ma) {
    if (!pmu_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    current_pmu_data.charge_current = current_ma;
    printf("PMU Service: Set charge current to %u mA\n", current_ma);
    return ESP_OK;
}

esp_err_t pmu_service_set_charge_voltage(uint16_t voltage_mv) {
    if (!pmu_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    current_pmu_data.charge_voltage = voltage_mv;
    printf("PMU Service: Set charge voltage to %u mV\n", voltage_mv);
    return ESP_OK;
}

esp_err_t pmu_service_set_power_channel(const char *channel, bool enable) {
    if (!pmu_initialized || !channel) {
        return ESP_ERR_INVALID_STATE;
    }

    // 根据通道名称设置对应的状态
    if (strcmp(channel, "dc1") == 0) {
        current_pmu_data.dc1_enabled = enable;
    } else if (strcmp(channel, "dc3") == 0) {
        current_pmu_data.dc3_enabled = enable;
    } else if (strcmp(channel, "aldo1") == 0) {
        current_pmu_data.aldo1_enabled = enable;
    } else if (strcmp(channel, "aldo2") == 0) {
        current_pmu_data.aldo2_enabled = enable;
    } else if (strcmp(channel, "aldo3") == 0) {
        current_pmu_data.aldo3_enabled = enable;
    } else if (strcmp(channel, "aldo4") == 0) {
        current_pmu_data.aldo4_enabled = enable;
    } else if (strcmp(channel, "bldo1") == 0) {
        current_pmu_data.bldo1_enabled = enable;
    } else if (strcmp(channel, "bldo2") == 0) {
        current_pmu_data.bldo2_enabled = enable;
    } else {
        return ESP_ERR_INVALID_ARG;
    }

    printf("PMU Service: Set power channel %s to %s\n", channel, enable ? "enabled" : "disabled");
    return ESP_OK;
}

esp_err_t pmu_service_reset(void) {
    if (!pmu_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    // 重置到初始状态
    init_mock_data();
    printf("PMU Service: Reset to initial state\n");
    return ESP_OK;
}

// 辅助函数：检查服务是否可用
bool pmu_service_is_available(void) {
    return pmu_initialized && (current_pmu_status == PMU_STATUS_CONNECTED);
}

// 辅助函数：检查USB是否连接
bool pmu_service_is_usb_connected(void) {
    return current_pmu_data.vbus_present;
}

// 辅助函数：检查电池是否在充电
bool pmu_service_is_battery_charging(void) {
    return (current_pmu_data.battery_status == BATTERY_STATUS_CHARGING);
}

// 辅助函数：获取电池电量百分比
int pmu_service_get_battery_percentage(void) {
    if (!pmu_initialized) {
        return -1;
    }
    return current_pmu_data.battery_percentage;
}

// 辅助函数：获取电池电压（伏特）
float pmu_service_get_battery_voltage(void) {
    if (!pmu_initialized) {
        return 0.0f;
    }
    return current_pmu_data.battery_voltage / 1000.0f;
}
