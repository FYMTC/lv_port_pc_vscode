// Mock SOC capabilities for Windows compatibility
#pragma once

// Touch sensor capabilities
#define SOC_TOUCH_SENSOR_NUM            10
#define SOC_TOUCH_PROXIMITY_CHANNEL_NUM 3
#define SOC_TOUCH_PROXIMITY_MEAS_DONE_SUPPORTED 1

// GPIO capabilities
#define SOC_GPIO_PIN_COUNT              40

// Other mock capabilities
#define SOC_RTC_SLOW_MEM_SUPPORTED      1
#define SOC_RTC_FAST_MEM_SUPPORTED      1

#define SOC_PM_SUPPORT_EXT_WAKEUP       1
#define SOC_PM_SUPPORT_TOUCH_SENSOR_WAKEUP 1
#define SOC_PM_SUPPORT_RTC_PERIPH_PD    1
#define SOC_PM_SUPPORT_RTC_SLOW_MEM_PD  1
#define SOC_PM_SUPPORT_RTC_FAST_MEM_PD  1
#define SOC_PM_SUPPORT_VDDSDIO_PD       1
