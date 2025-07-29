// Mock HAL touch sensor types for Windows compatibility
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TOUCH_PAD_0 = 0,    /*!< Touch pad channel 0 is GPIO4(ESP32) */
    TOUCH_PAD_1,        /*!< Touch pad channel 1 is GPIO0(ESP32) / GPIO1(ESP32-S2) */
    TOUCH_PAD_2,        /*!< Touch pad channel 2 is GPIO2(ESP32) / GPIO2(ESP32-S2) */
    TOUCH_PAD_3,        /*!< Touch pad channel 3 is GPIO15(ESP32) / GPIO3(ESP32-S2) */
    TOUCH_PAD_MAX,
} touch_pad_t;

typedef enum {
    TOUCH_TRIGGER_BELOW = 0,   /*!< Touch interrupt will happen if counter value is less than threshold.*/
    TOUCH_TRIGGER_ABOVE = 1,   /*!< Touch interrupt will happen if counter value is larger than threshold.*/
    TOUCH_TRIGGER_MAX,
} touch_trigger_mode_t;

#ifdef __cplusplus
}
#endif
