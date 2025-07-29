
#include "my_ui.h"
#include "lvgl/lvgl.h"

#include "page_manager.h"
#include "pages_common.h"
#include "system/system.h"

// 全局页面管理器
PageManager g_pageManager;


extern "C" void my_ui_init(void)
{
    // 初始化硬件资源
    time_service::init();
    battery_service::init();
    wifi_manager_init();
    mpu6050_service_init();
    qmc5883l_service_init();
    pmu_service_init();


    // 注册页面（直接绑定事件）
    g_pageManager.registerPage("pre_page", createPage_prepage); // 暂时禁用，缺少Lottie数据
    g_pageManager.registerPage("page_menu", createPage_menu);
    g_pageManager.registerPage("page_settings", createPage_settings);
    g_pageManager.registerPage("page_mpu6050", createPage_mpu6050);
    g_pageManager.registerPage("page_qmc5883l", createPage_qmc5883l);
    g_pageManager.registerPage("page_time", createPage_time);
    g_pageManager.registerPage("page_wifi", createPage_wifi);
    g_pageManager.registerPage("page_pmu", createPage_pmu);
    g_pageManager.registerPage("page_sd_files", createPage_sd_files);
    g_pageManager.registerPage("page1", createPage1);
    g_pageManager.registerPage("page2", createPage2);
    // 启动时加载主菜单页面
    g_pageManager.gotoPage("pre_page");
}
