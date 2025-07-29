#include "mpu6050_service.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

static mpu6050_data_callback_t data_callback = nullptr;
static mpu6050_status_t current_status = {false, false, 0, 0};
static mpu6050_data_t current_data = {0};

esp_err_t mpu6050_service_init(void) {
    current_status.is_initialized = true;
    current_status.is_connected = true;
    current_status.error_count = 0;
    current_status.last_update = 0;

    srand(time(nullptr));
    return ESP_OK;
}

void mpu6050_service_deinit(void) {
    current_status.is_initialized = false;
    current_status.is_connected = false;
    data_callback = nullptr;
}

esp_err_t mpu6050_read_data(mpu6050_data_t* data) {
    if (!current_status.is_initialized || !data) {
        return ESP_ERR_INVALID_STATE;
    }

    // 模拟传感器数据
    static float base_accel_x = 0.0f;
    static float base_accel_y = 0.0f;
    static float base_accel_z = 9.8f; // 重力加速度

    // 模拟设备倾斜，产生更大的角度变化
    static float tilt_angle_x = 0.0f;
    static float tilt_angle_y = 0.0f;
    static float rotation_speed = 0.1f; // 旋转速度

    // 更新倾斜角度（产生连续的角度变化）
    tilt_angle_x += ((rand() % 200 - 100) / 1000.0f) * rotation_speed;
    tilt_angle_y += ((rand() % 200 - 100) / 1000.0f) * rotation_speed;

    // 限制倾斜角度范围
    if (tilt_angle_x > 0.5f) tilt_angle_x = 0.5f;
    if (tilt_angle_x < -0.5f) tilt_angle_x = -0.5f;
    if (tilt_angle_y > 0.5f) tilt_angle_y = 0.5f;
    if (tilt_angle_y < -0.5f) tilt_angle_y = -0.5f;

    // 根据倾斜角度计算加速度计数据（模拟重力在各轴的分量）
    data->accel_x = base_accel_x + sinf(tilt_angle_x) * 9.8f + ((rand() % 200 - 100) / 1000.0f);
    data->accel_y = base_accel_y + sinf(tilt_angle_y) * 9.8f + ((rand() % 200 - 100) / 1000.0f);
    data->accel_z = base_accel_z + cosf(tilt_angle_x) * cosf(tilt_angle_y) * 9.8f + ((rand() % 200 - 100) / 1000.0f);

    // 增大陀螺仪数据范围（-50到+50度/秒）
    data->gyro_x = (rand() % 10000 - 5000) / 100.0f;
    data->gyro_y = (rand() % 10000 - 5000) / 100.0f;
    data->gyro_z = (rand() % 10000 - 5000) / 100.0f;

    data->temperature = 25.0f + (rand() % 1000 - 500) / 100.0f; // 20-30°C with more variation
    data->is_valid = true;
    data->timestamp = clock();

    current_data = *data;
    current_status.last_update = data->timestamp;

    return ESP_OK;
}

mpu6050_data_t mpu6050_get_last_data(void) {
    return current_data;
}

mpu6050_status_t mpu6050_get_status(void) {
    return current_status;
}

void mpu6050_set_data_callback(mpu6050_data_callback_t callback) {
    data_callback = callback;
}

void mpu6050_remove_data_callback(void) {
    data_callback = nullptr;
}

esp_err_t mpu6050_start_continuous_read(uint32_t interval_ms) {
    if (!current_status.is_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    // 模拟连续读取 - 在实际实现中这里会启动定时器
    return ESP_OK;
}

esp_err_t mpu6050_stop_continuous_read(void) {
    return ESP_OK;
}

bool mpu6050_is_available(void) {
    return current_status.is_initialized && current_status.is_connected;
}

void mpu6050_update_data(void) {
    if (!current_status.is_initialized) return;

    mpu6050_data_t data;
    if (mpu6050_read_data(&data) == ESP_OK && data_callback) {
        data_callback(&data);
    }
}

esp_err_t mpu6050_service_start(uint32_t interval_ms) {
    // Mock implementation - just mark as started
    current_status.is_initialized = true;
    current_status.is_connected = true;
    return ESP_OK;
}

esp_err_t mpu6050_service_calibrate(void) {
    // Mock implementation
    return ESP_OK;
}

esp_err_t mpu6050_service_reset(void) {
    // Mock implementation
    current_status.error_count = 0;
    return ESP_OK;
}

// 添加缺失的函数实现
esp_err_t mpu6050_service_register_callback(mpu6050_data_callback_t callback) {
    data_callback = callback;
    return ESP_OK;
}

void mpu6050_service_unregister_callback(void) {
    data_callback = nullptr;
}

esp_err_t mpu6050_service_get_data(mpu6050_data_t* data) {
    return mpu6050_read_data(data);
}

esp_err_t mpu6050_service_get_status(mpu6050_status_t* status) {
    if (!status) return ESP_ERR_INVALID_ARG;
    *status = current_status;
    return ESP_OK;
}
