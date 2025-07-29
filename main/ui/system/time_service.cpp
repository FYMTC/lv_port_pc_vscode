#include "time_service.h"
#include <ctime>
#include <cstring>
#include <chrono>

namespace time_service {

static TimeUpdateCallback time_callback = nullptr;
static bool service_initialized = false;
static bool time_synced = false;
static auto start_time = std::chrono::steady_clock::now();

void init() {
    service_initialized = true;
    time_synced = true; // 模拟时间已同步
}

void deinit() {
    service_initialized = false;
    time_callback = nullptr;
}

TimeInfo get_time_info() {
    TimeInfo info = {};

    if (!service_initialized) {
        strcpy(info.time_str, "--:--");
        strcpy(info.datetime_str, "----:--:-- --:--:--");
        strcpy(info.running_str, "--:--:--");
        info.is_synced = false;
        info.timestamp = 0;
        return info;
    }

    // 获取当前时间
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    // 格式化时间字符串
    strftime(info.time_str, sizeof(info.time_str), "%H:%M", &tm);
    strftime(info.datetime_str, sizeof(info.datetime_str), "%Y-%m-%d %H:%M:%S", &tm);

    // 计算运行时间
    auto current_time = std::chrono::steady_clock::now();
    auto running_duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);

    int hours = running_duration.count() / 3600;
    int minutes = (running_duration.count() % 3600) / 60;
    int seconds = running_duration.count() % 60;

    snprintf(info.running_str, sizeof(info.running_str), "%02d:%02d:%02d", hours, minutes, seconds);

    info.is_synced = time_synced;
    info.timestamp = now;

    return info;
}

void set_time_update_callback(TimeUpdateCallback callback) {
    time_callback = callback;
}

void remove_time_update_callback() {
    time_callback = nullptr;
}

bool is_time_synced() {
    return time_synced;
}

const char* format_running_time(unsigned long millis) {
    static char buffer[32];
    unsigned long seconds = millis / 1000;
    unsigned long hours = seconds / 3600;
    unsigned long minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;

    snprintf(buffer, sizeof(buffer), "%02lu:%02lu:%02lu", hours, minutes, seconds);
    return buffer;
}

void start_sntp_sync() {
    // 模拟SNTP同步
    time_synced = true;
}

void stop_sntp_sync() {
    // 停止SNTP同步
}

bool is_sntp_syncing() {
    return false; // 模拟非同步状态
}

bool is_rtc_available() {
    return true; // 模拟RTC可用
}

bool sync_rtc_from_system() {
    return true; // 模拟同步成功
}

bool get_rtc_time(struct tm* rtc_time) {
    if (!rtc_time) return false;

    auto now = std::time(nullptr);
    *rtc_time = *std::localtime(&now);
    return true;
}

// 辅助函数：更新时间信息（供UI调用）
void update_time_info() {
    if (!service_initialized || !time_callback) return;

    TimeInfo info = get_time_info();
    time_callback(info);
}

} // namespace time_service
