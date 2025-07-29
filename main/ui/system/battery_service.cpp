#include "battery_service.h"
#include <cstdlib>
#include <ctime>

namespace battery_service {

static BatteryUpdateCallback battery_callback = nullptr;
static LowBatteryWarningCallback warning_callback = nullptr;
static BatteryInfo current_battery_info = {75, false, true, 3800, true};
static LowBatteryConfig battery_config = {5, 15, true};
static bool service_initialized = false;

bool init() {
    service_initialized = true;
    srand(time(nullptr));
    return true;
}

void deinit() {
    service_initialized = false;
    battery_callback = nullptr;
    warning_callback = nullptr;
}

BatteryInfo get_battery_info() {
    if (!service_initialized) {
        return {0, false, false, 0, false};
    }

    // 模拟电池电量变化
    static int last_percentage = current_battery_info.percentage;

    // 随机小幅变化电量
    int change = (rand() % 3) - 1; // -1, 0, 1
    current_battery_info.percentage = std::max(0, std::min(100, last_percentage + change));

    // 模拟充电状态
    current_battery_info.is_charging = (rand() % 10) < 2; // 20% 概率充电

    // 根据电量计算电压
    current_battery_info.voltage_mv = 3300 + (current_battery_info.percentage * 7); // 3.3V - 4.0V

    return current_battery_info;
}

void set_battery_update_callback(BatteryUpdateCallback callback) {
    battery_callback = callback;
}

void remove_battery_update_callback() {
    battery_callback = nullptr;
}

void set_low_battery_warning_callback(LowBatteryWarningCallback callback) {
    warning_callback = callback;
}

void configure_low_battery_shutdown(const LowBatteryConfig& config) {
    battery_config = config;
}

LowBatteryConfig get_low_battery_config() {
    return battery_config;
}

bool is_available() {
    return service_initialized;
}

void update_battery_info() {
    if (!service_initialized) return;

    BatteryInfo info = get_battery_info();

    if (battery_callback) {
        battery_callback(info);
    }

    if (warning_callback && info.percentage <= battery_config.warning_percentage) {
        warning_callback(info.percentage);
    }
}

} // namespace battery_service
