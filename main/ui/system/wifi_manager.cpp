#include "wifi_manager.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "esp_log.h"
static const char* TAG = "WiFiManager";
// 全局状态变量
static wifi_manager_status_t current_status = WIFI_MANAGER_DISCONNECTED;
static wifi_manager_config_t current_config = {};
static wifi_manager_status_cb_t status_callback = nullptr;
static wifi_manager_sntp_cb_t sntp_callback = nullptr;
static bool initialized = false;

// 模拟的扫描结果
static wifi_manager_ap_info_t mock_scan_results[] = {
    {"MockWiFi-5G", -35, WIFI_AUTH_WPA2_PSK, false},
    {"MockWiFi-2.4G", -45, WIFI_AUTH_WPA2_PSK, false},
    {"OpenNetwork", -55, WIFI_AUTH_OPEN, true},
    {"SecureNet", -65, WIFI_AUTH_WPA3_PSK, false},
    {"TestNetwork", -75, WIFI_AUTH_WPA_WPA2_PSK, false}
};

esp_err_t wifi_manager_init(void) {
    if (initialized) {
        return ESP_OK;
    }

    initialized = true;
    current_status = WIFI_MANAGER_DISCONNECTED;
    srand(time(nullptr));

    return ESP_OK;
}

wifi_manager_status_t wifi_manager_get_status(void) {
    return current_status;
}

void wifi_manager_register_status_cb(wifi_manager_status_cb_t cb) {
    status_callback = cb;
}

void wifi_manager_register_sntp_cb(wifi_manager_sntp_cb_t cb) {
    sntp_callback = cb;
}

esp_err_t wifi_manager_connect_with_config(const wifi_manager_config_t* config) {
    if (!initialized || !config) {
        return ESP_FAIL;
    }

    // 保存配置
    current_config = *config;

    // 模拟连接过程
    current_status = WIFI_MANAGER_CONNECTING;
    if (status_callback) {
        status_callback(current_status, nullptr);
    }

    // 模拟连接成功/失败
    bool connection_success = (rand() % 10) > 1; // 90% 成功率

    if (connection_success) {
        current_status = WIFI_MANAGER_CONNECTED;

        // 模拟时间同步
        current_status = WIFI_MANAGER_TIME_SYNCING;
        if (status_callback) {
            status_callback(current_status, nullptr);
        }

        // 模拟时间同步完成
        current_status = WIFI_MANAGER_TIME_SYNCED;
        if (sntp_callback) {
            sntp_callback(true);
        }
    } else {
        current_status = WIFI_MANAGER_CONNECTION_FAILED;
    }

    if (status_callback) {
        status_callback(current_status, nullptr);
    }

    return ESP_OK;
}

esp_err_t wifi_manager_connect_to_ap(const char* ssid, const char* password) {
    if (!ssid) {
        return ESP_FAIL;
    }

    wifi_manager_config_t config = {};
    strncpy(config.ssid, ssid, sizeof(config.ssid) - 1);
    if (password) {
        strncpy(config.password, password, sizeof(config.password) - 1);
    }
    config.maximum_retry = 5;
    config.auto_reconnect = true;

    return wifi_manager_connect_with_config(&config);
}

void wifi_manager_disconnect(void) {
    current_status = WIFI_MANAGER_DISCONNECTED;
    memset(&current_config, 0, sizeof(current_config));

    if (status_callback) {
        status_callback(current_status, nullptr);
    }
}

bool wifi_manager_is_connected(void) {
    return (current_status == WIFI_MANAGER_CONNECTED ||
            current_status == WIFI_MANAGER_TIME_SYNCING ||
            current_status == WIFI_MANAGER_TIME_SYNCED);
}

esp_err_t wifi_manager_get_current_ssid(char* ssid_buf, size_t buf_len) {
    if (!ssid_buf || buf_len == 0) {
        return ESP_FAIL;
    }

    if (!wifi_manager_is_connected()) {
        return ESP_FAIL;
    }

    strncpy(ssid_buf, current_config.ssid, buf_len - 1);
    ssid_buf[buf_len - 1] = '\0';

    return ESP_OK;
}

esp_err_t wifi_manager_get_current_ip(char* ip_buf, size_t buf_len) {
    if (!ip_buf || buf_len == 0) {
        return ESP_FAIL;
    }

    if (!wifi_manager_is_connected()) {
        return ESP_FAIL;
    }

    // 模拟IP地址
    sprintf(ip_buf, "192.168.1.%d", 100 + (rand() % 50));

    return ESP_OK;
}

int8_t wifi_manager_get_rssi(void) {
    if (!wifi_manager_is_connected()) {
        return 0;
    }

    // 模拟信号强度
    return -30 - (rand() % 40); // -30 到 -70 dBm
}

esp_err_t wifi_manager_start_scan(bool block) {
    if (!initialized) {
        return ESP_FAIL;
    }

    // 模拟扫描过程，block参数在模拟环境中被忽略
    return ESP_OK;
}

esp_err_t wifi_manager_get_scan_results(wifi_manager_ap_info_t* ap_list,
                                       uint16_t max_count,
                                       uint16_t* actual_count) {
    if (!ap_list || max_count == 0 || !actual_count) {
        return ESP_ERR_INVALID_ARG;
    }

    int count = 5; // 直接使用常量，因为我们有5个模拟AP
    count = (count > max_count) ? max_count : count;

    for (int i = 0; i < count; i++) {
        ap_list[i] = mock_scan_results[i];
        // 添加随机信号强度变化
        ap_list[i].rssi += (rand() % 10 - 5);
    }

    *actual_count = count;
    return ESP_OK;
}

esp_err_t wifi_manager_save_config(const wifi_manager_config_t* config) {
    if (!config) {
        return ESP_FAIL;
    }

    // 模拟保存配置到NVS
    return ESP_OK;
}

esp_err_t wifi_manager_load_config(wifi_manager_config_t* config) {
    if (!config) {
        return ESP_FAIL;
    }

    // 模拟从NVS加载配置
    strncpy(config->ssid, "SavedNetwork", sizeof(config->ssid) - 1);
    strncpy(config->password, "SavedPassword", sizeof(config->password) - 1);
    config->maximum_retry = 5;
    config->auto_reconnect = true;

    return ESP_OK;
}

void wifi_manager_clear_config(void) {
    // 模拟清除NVS中的配置
    memset(&current_config, 0, sizeof(current_config));
}

// 添加缺失的函数实现
esp_err_t wifi_manager_get_connected_ssid(char* ssid_buf, size_t buf_len) {
    if (!ssid_buf || buf_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    if (current_status != WIFI_MANAGER_CONNECTED) {
        ssid_buf[0] = '\0'; // 空字符串表示未连接
        return ESP_FAIL;
    }

    // 返回当前连接的SSID
    strncpy(ssid_buf, current_config.ssid, buf_len - 1);
    ssid_buf[buf_len - 1] = '\0';

    return ESP_OK;
}

esp_err_t wifi_manager_register_status_callback(wifi_manager_status_cb_t callback) {
    status_callback = callback;
    return ESP_OK;
}

esp_err_t wifi_manager_unregister_status_callback(void) {
    status_callback = nullptr;
    return ESP_OK;
}
esp_err_t wifi_manager_enable(void)
{
    ESP_LOGI(TAG, "Enabling WiFi...");
    esp_err_t ret = ESP_OK;
    return ret;
}

esp_err_t wifi_manager_disable(void)
{
    ESP_LOGI(TAG, "Disabling WiFi...");
    esp_err_t ret = ESP_OK;
    return ret;
}

bool wifi_manager_is_enabled(void)
{
    bool enabled;
    //随机返回WiFi是否启用
    enabled = (rand() % 2) == 0; // 50%概率启用
    ESP_LOGI(TAG, "WiFi is %s", enabled ? "enabled" : "disabled");
    return enabled;
}
