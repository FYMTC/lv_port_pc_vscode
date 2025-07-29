#include "sd_init_mock.hpp"

static sd_card_status_t card_status = SD_CARD_STATUS_MOUNTED;
static sd_card_info_t card_info = {
    "MOCK_SD",
    32ULL * 1024 * 1024 * 1024, // 32GB
    512,
    true,
    "/sd"
};

esp_err_t sd_card_init(void) {
    card_status = SD_CARD_STATUS_MOUNTED;
    return ESP_OK;
}

void sd_card_deinit(void) {
    card_status = SD_CARD_STATUS_NOT_PRESENT;
}

sd_card_status_t sd_card_get_status(void) {
    return card_status;
}

esp_err_t sd_card_get_info(sd_card_info_t* info) {
    if (!info) {
        return ESP_ERR_INVALID_ARG;
    }

    *info = card_info;
    return ESP_OK;
}

bool sd_card_is_available(void) {
    return (card_status == SD_CARD_STATUS_MOUNTED);
}
