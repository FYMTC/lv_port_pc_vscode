// page1.cpp
#include "lvgl/lvgl.h"
#include "page_manager.h"
#include "pages_common.h"
#include <iostream>
extern PageManager g_pageManager;

#define CANVAS_WIDTH  240
#define CANVAS_HEIGHT  200
lv_obj_t * chart = NULL;
lv_obj_t * canvas = NULL;
static void timer_cb(lv_timer_t * timer)
{
    static int16_t counter = 0;
    const char * string = "lol~ I'm wavvvvvvving~>>>";
    const int16_t string_len = lv_strlen(string);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

    lv_draw_letter_dsc_t letter_dsc;
    lv_draw_letter_dsc_init(&letter_dsc);
    letter_dsc.color = lv_color_hex(0xff0000);
    letter_dsc.font = lv_font_get_default();

    {
#define CURVE2_X(t) (t * 2 + 10)
#define CURVE2_Y(t) (lv_trigo_sin((t) * 5) * 40 / 32767 + CANVAS_HEIGHT / 2)

        int32_t pre_x = CURVE2_X(-1);
        int32_t pre_y = CURVE2_Y(-1);
        for(int16_t i = 0; i < string_len; i++) {
            const int16_t angle = (int16_t)(i * 5);
            const int32_t x = CURVE2_X(angle);
            const int32_t y = CURVE2_Y(angle + counter / 2);
            const lv_point_t point = { .x = x, .y = y };

            letter_dsc.unicode = (uint32_t)string[i % string_len];
            letter_dsc.rotation = lv_atan2(y - pre_y, x - pre_x) * 10;
            letter_dsc.color = lv_color_hsv_to_rgb(i * 10, 100, 100);
            lv_draw_letter(&layer, &letter_dsc, &point);

            pre_x = x;
            pre_y = y;
        }
    }

    lv_chart_series_t * ser = lv_chart_get_series_next(chart, NULL);
    lv_chart_set_next_value(chart, ser, (int32_t)lv_rand(10, 90));
    uint32_t p = lv_chart_get_point_count(chart);
    uint32_t s = lv_chart_get_x_start_point(chart, ser);
    int32_t * a = lv_chart_get_series_y_array(chart, ser);
    a[(s + 1) % p] = LV_CHART_POINT_NONE;
    a[(s + 2) % p] = LV_CHART_POINT_NONE;
    a[(s + 2) % p] = LV_CHART_POINT_NONE;

    lv_chart_refresh(chart);
    lv_canvas_finish_layer(canvas, &layer);
    counter++;
}

static void screen_backbtn_cb(lv_event_t *e) {
    g_pageManager.gotoPage("page_menu", LV_SCR_LOAD_ANIM_FADE_OUT, 300);
    std::cout<<"Back button clicked!"<<std::endl;
}

lv_obj_t* createPage1() {
    LV_DRAW_BUF_DEFINE_STATIC(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
    LV_DRAW_BUF_INIT_STATIC(draw_buf);

    lv_obj_t * page1 = lv_obj_create(NULL);
    lv_obj_t *status = lv_obj_create(page1);
    lv_obj_set_size(status, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_pad_ver(status, 0, 0);
    lv_obj_set_flex_flow(status, LV_FLEX_FLOW_COLUMN); // 按钮内容弹性行增长
    lv_obj_set_flex_align(status, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_text_font(status, &lv_font_montserrat_14, 0);
    lv_obj_set_style_border_width(status, 0, 0);
    lv_obj_align(status, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *screen_backbtn = lv_btn_create(status);
    lv_obj_set_size(screen_backbtn, LV_PCT(100), 20);
    lv_obj_t *img = lv_img_create(screen_backbtn);
    lv_img_set_src(img, LV_SYMBOL_NEW_LINE);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_t *label = lv_label_create(screen_backbtn);
    lv_label_set_text(label, "BACK");
    lv_obj_align_to(label, img, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(screen_backbtn, screen_backbtn_cb, LV_EVENT_CLICKED, NULL);


    canvas = lv_canvas_create(status);
    lv_obj_set_size(canvas, CANVAS_WIDTH, CANVAS_HEIGHT);
    lv_obj_center(canvas);
    lv_canvas_set_draw_buf(canvas, &draw_buf);

    chart = lv_chart_create(status);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);
    lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_size(chart, LV_PCT(100), 150);
    lv_obj_center(chart);

    lv_chart_set_point_count(chart, 100);
    lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    /*Prefill with data*/
    uint32_t i;
    for(i = 0; i < 100; i++) {
        lv_chart_set_next_value(chart, ser, (int32_t)lv_rand(10, 90));
    }



    lv_timer_t * timer = lv_timer_create(timer_cb, 10, NULL);
    lv_obj_set_user_data(page1, timer);

    return page1;
}
