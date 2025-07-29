// Mock ESP32 error codes and basic definitions
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef int esp_err_t;

#define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM              0x101   /*!< Out of memory */
#define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define ESP_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define ESP_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define ESP_ERR_NOT_FOUND          0x105   /*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED      0x106   /*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT            0x107   /*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE   0x108   /*!< Invalid response */
#define ESP_ERR_INVALID_CRC        0x109   /*!< Invalid CRC */
#define ESP_ERR_INVALID_VERSION    0x10A   /*!< Invalid version */
#define ESP_ERR_INVALID_MAC        0x10B   /*!< Invalid MAC address */
#define ESP_ERR_NOT_FINISHED       0x10C   /*!< Operation not finished */

#ifdef __cplusplus
}
#endif
