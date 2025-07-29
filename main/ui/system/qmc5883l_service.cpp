#include "qmc5883l_service.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static qmc5883l_data_callback_t data_callback = nullptr;
static qmc5883l_status_t current_status = {false, false, 0, 0};
static qmc5883l_service_data_t current_data = {0};

esp_err_t qmc5883l_service_init(void) {
    current_status.is_initialized = true;
    current_status.is_connected = true;
    current_status.error_count = 0;
    current_status.last_update = 0;

    srand(time(nullptr));
    return ESP_OK;
}

void qmc5883l_service_deinit(void) {
    current_status.is_initialized = false;
    current_status.is_connected = false;
    data_callback = nullptr;
}

esp_err_t qmc5883l_service_read_data(qmc5883l_service_data_t* data) {
    if (!current_status.is_initialized || !data) {
        return ESP_ERR_INVALID_STATE;
    }

    // 模拟磁力计数据
    static float base_heading = 0.0f;
    static float heading_change_rate = 1.0f; // 度/更新

    // 模拟磁场数据 (单位: mG) - 增加变化范围
    data->mag_x = 200.0f + (rand() % 200 - 100);  // 100-300 mG
    data->mag_y = 150.0f + (rand() % 200 - 100);  // 50-250 mG
    data->mag_z = -400.0f + (rand() % 200 - 100); // -500 to -300 mG

    // 计算磁场强度
    data->magnitude = sqrt(data->mag_x * data->mag_x +
                          data->mag_y * data->mag_y +
                          data->mag_z * data->mag_z);

    // 添加更大幅度的航向角度变化
    base_heading += (rand() % 20 - 10) * heading_change_rate; // ±10度变化
    if (base_heading < 0) base_heading += 360.0f;
    if (base_heading >= 360.0f) base_heading -= 360.0f;

    // 使用模拟的航向角
    data->heading = base_heading;
    data->is_valid = true;
    data->timestamp = clock();

    current_data = *data;
    current_status.last_update = data->timestamp;

    return ESP_OK;
}

qmc5883l_service_data_t qmc5883l_service_get_last_data(void) {
    return current_data;
}

qmc5883l_status_t qmc5883l_service_get_status(void) {
    return current_status;
}

void qmc5883l_service_set_data_callback(qmc5883l_data_callback_t callback) {
    data_callback = callback;
}

void qmc5883l_service_remove_data_callback(void) {
    data_callback = nullptr;
}

esp_err_t qmc5883l_service_start_continuous_read(uint32_t interval_ms) {
    if (!current_status.is_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    // 模拟连续读取启动
    return ESP_OK;
}

esp_err_t qmc5883l_service_stop_continuous_read(void) {
    return ESP_OK;
}

bool qmc5883l_service_is_available(void) {
    return current_status.is_initialized && current_status.is_connected;
}

void qmc5883l_service_update_data(void) {
    if (!current_status.is_initialized) return;

    qmc5883l_service_data_t data;
    if (qmc5883l_service_read_data(&data) == ESP_OK && data_callback) {
        data_callback(&data);
    }
}

float qmc5883l_service_get_heading(void) {
    if (!current_status.is_initialized) {
        return 0.0f;
    }

    qmc5883l_service_data_t data;
    if (qmc5883l_service_read_data(&data) == ESP_OK) {
        return data.heading;
    }

    return 0.0f;
}

// 添加缺失的函数实现
esp_err_t qmc5883l_service_start(uint32_t update_interval_ms) {
    return qmc5883l_service_start_continuous_read(update_interval_ms);
}

esp_err_t qmc5883l_service_get_data(qmc5883l_service_data_t* data) {
    return qmc5883l_service_read_data(data);
}

esp_err_t qmc5883l_service_get_status(qmc5883l_status_t* status) {
    if (!status) return ESP_ERR_INVALID_ARG;
    *status = current_status;
    return ESP_OK;
}

esp_err_t qmc5883l_service_register_callback(qmc5883l_data_callback_t callback) {
    data_callback = callback;
    return ESP_OK;
}

void qmc5883l_service_unregister_callback(void) {
    data_callback = nullptr;
}

esp_err_t qmc5883l_service_calibrate(void) {
    // 模拟校准过程
    return ESP_OK;
}

esp_err_t qmc5883l_service_reset(void) {
    // 模拟复位过程
    current_status.error_count = 0;
    return ESP_OK;
}
