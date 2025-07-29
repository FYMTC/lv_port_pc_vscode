#ifndef ESP_ERR_TO_NAME_H
#define ESP_ERR_TO_NAME_H

#include "esp_err.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Windows compatibility for strcasecmp
#ifdef _WIN32
#define strcasecmp _stricmp
#endif

/**
 * @brief Convert ESP error code to human readable string
 * @param code Error code
 * @return Human readable error string
 */
const char* esp_err_to_name(esp_err_t code);

#ifdef __cplusplus
}
#endif

#endif // ESP_ERR_TO_NAME_H
