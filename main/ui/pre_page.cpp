#include "lvgl/lvgl.h"
#include "page_manager.h"
#include "pages_common.h"
#include "access/MtkNxUsr7K.c"
extern PageManager g_pageManager;

static void lottie_anim_completed_cb(lv_anim_t *a)
{
    g_pageManager.gotoPageAndDestroy("page_menu", LV_SCR_LOAD_ANIM_FADE_IN, 300);
}

lv_obj_t* createPage_prepage(void)
{
    // 声明Lottie动画数据
    // extern const uint8_t lv_example_lottie_approve[];
    // extern const size_t lv_example_lottie_approve_size;

    lv_obj_t * pre_page = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(pre_page, lv_color_black(), 0); // 设置背景为黑色
    lv_obj_t * lottie = lv_lottie_create(pre_page);
    //lv_lottie_set_src_data(lottie, lv_example_lottie_approve, lv_example_lottie_approve_size);
    lv_lottie_set_src_data(lottie, MtkNxUsr7K, MtkNxUsr7K_len);
    static uint8_t buf[240 * 240 * 4];
    lv_lottie_set_buffer(lottie, 240, 240, buf);
    lv_obj_center(lottie);

    lv_anim_t *a = lv_lottie_get_anim(lottie);
	a->repeat_cnt=1;
    if (a) {
        a->completed_cb = lottie_anim_completed_cb;
    }

    return pre_page;
}
