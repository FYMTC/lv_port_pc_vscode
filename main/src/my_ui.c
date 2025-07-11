// my_ui.c
// 示例：自定义 LVGL UI 初始化
#include "my_ui.h"
#include "lvgl/lvgl.h"

void my_ui_init(void)
{
    // 创建一个简单的标签控件
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello, LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    // 你可以在这里添加更多控件和布局
}
