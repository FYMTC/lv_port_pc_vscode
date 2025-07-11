
# 设置 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.12.4)
# 定义项目名称和支持的语言
project(lvgl C CXX)


# 设置 FreeRTOS 的端口类型（如在 WSL 下使用 Posix）
set(FREERTOS_PORT GCC_POSIX CACHE STRING "Port for FreeRTOS on Posix environment")


# 取消之前的 USE_FREERTOS 设置，重新定义是否启用 FreeRTOS
unset(USE_FREERTOS CACHE)
# 是否启用 FreeRTOS，默认关闭
option(USE_FREERTOS "Enable FreeRTOS" OFF) # Turn this on to enable FreeRTOS


# 根据 USE_FREERTOS 选项决定是否集成 FreeRTOS
if(USE_FREERTOS)
    message(STATUS "FreeRTOS is enabled") # 启用 FreeRTOS

    # 创建 FreeRTOS 配置库（接口库）
    add_library(freertos_config INTERFACE)
    # 添加 FreeRTOS 配置头文件路径
    target_include_directories(freertos_config SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/config)
    # 定义编译宏
    target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)

    # 添加 FreeRTOS 子目录
    add_subdirectory(FreeRTOS)

    # 添加 FreeRTOS 相关头文件路径
    include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/include)
    include_directories(${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix)
    include_directories(${PROJECT_SOURCE_DIR}/config)

    # 收集 FreeRTOS 源文件
    file(GLOB FREERTOS_SOURCES
        "${PROJECT_SOURCE_DIR}/FreeRTOS/*.c"
        "${PROJECT_SOURCE_DIR}/FreeRTOS/portable/MemMang/heap_4.c"
        "${PROJECT_SOURCE_DIR}/FreeRTOS/portable/ThirdParty/GCC/Posix/*.c"
    )
else()
    message(STATUS "FreeRTOS is disabled") # 未启用 FreeRTOS
    set(FREERTOS_SOURCES "")  # 未启用时不包含 FreeRTOS 源文件
endif()


# 项目主头文件路径
include_directories(${PROJECT_SOURCE_DIR}/main/inc)


# LVGL 相关功能选项，默认均为关闭
option(LV_USE_DRAW_SDL "Use SDL draw unit" OFF)           # 是否使用 SDL 绘图单元
option(LV_USE_LIBPNG "Use libpng to decode PNG" OFF)      # 是否使用 libpng 解码 PNG
option(LV_USE_LIBJPEG_TURBO "Use libjpeg turbo to decode JPEG" OFF) # 是否使用 libjpeg turbo 解码 JPEG
option(LV_USE_FFMPEG "Use libffmpeg to display video using lv_ffmpeg" OFF) # 是否使用 ffmpeg 显示视频
option(LV_USE_FREETYPE "Use freetype library" OFF)        # 是否使用 freetype 字体库


# 设置 C 和 C++ 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置工作目录
set(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})


# 查找并包含 SDL2 库
find_package(SDL2 REQUIRED)


# 移除 ARM 相关编译选项，使用本地架构优化
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")


# 根据选项添加编译宏定义
add_compile_definitions($<$<BOOL:${LV_USE_DRAW_SDL}>:LV_USE_DRAW_SDL=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBPNG}>:LV_USE_LIBPNG=1>)
add_compile_definitions($<$<BOOL:${LV_USE_LIBJPEG_TURBO}>:LV_USE_LIBJPEG_TURBO=1>)
add_compile_definitions($<$<BOOL:${LV_USE_FFMPEG}>:LV_USE_FFMPEG=1>)


# 添加 LVGL 子目录，并设置头文件路径
add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS})


# 创建主程序，根据是否启用 FreeRTOS 选择不同的源文件和链接库
if(USE_FREERTOS)
    add_executable(main
        ${PROJECT_SOURCE_DIR}/main/src/main.c
        ${PROJECT_SOURCE_DIR}/main/src/freertos_main.cpp
        ${PROJECT_SOURCE_DIR}/main/src/mouse_cursor_icon.c
        ${PROJECT_SOURCE_DIR}/main/src/FreeRTOS_Posix_Port.c
        ${FREERTOS_SOURCES}  # 仅在启用 FreeRTOS 时添加
    )
    # 链接 FreeRTOS 相关库
    target_link_libraries(main freertos_config FreeRTOS)
else()
    add_executable(main
        ${PROJECT_SOURCE_DIR}/main/src/main.c
        ${PROJECT_SOURCE_DIR}/main/src/mouse_cursor_icon.c
        ${PROJECT_SOURCE_DIR}/main/ui/my_ui.c  #添加自定义 UI 源文件
    )
endif()


# 定义 LVGL 配置为简单包含
target_compile_definitions(main PRIVATE LV_CONF_INCLUDE_SIMPLE)


# 根据编译器类型链接不同的库
if(MSVC)
    target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES})
else()
    target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
endif()


# 仅在启用 FreeRTOS 时链接 freertos_config 库
if(USE_FREERTOS)
    target_link_libraries(main freertos_config)
endif()


# 自定义目标，运行主程序
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)


# 如果启用 LV_USE_DRAW_SDL，则查找并链接 SDL2_image 库
if(LV_USE_DRAW_SDL)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
    target_include_directories(lvgl PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(main ${SDL2_IMAGE_LIBRARIES})
endif()


# 如果启用 LV_USE_LIBPNG，则查找并链接 libpng 库
if(LV_USE_LIBPNG)
    find_package(PNG REQUIRED)
    target_include_directories(lvgl PUBLIC ${PNG_INCLUDE_DIRS})
    target_link_libraries(main ${PNG_LIBRARIES})
endif()


# 如果启用 LV_USE_LIBJPEG_TURBO，则查找并链接 libjpeg-turbo 库
if(LV_USE_LIBJPEG_TURBO)
    find_package(JPEG REQUIRED)
    target_include_directories(lvgl PUBLIC ${JPEG_INCLUDE_DIRS})
    target_link_libraries(main ${JPEG_LIBRARIES})
endif()


# 如果启用 LV_USE_FFMPEG，则链接 FFmpeg 相关库
if(LV_USE_FFMPEG)
    target_link_libraries(main avformat avcodec avutil swscale z)
endif()


# 如果启用 LV_USE_FREETYPE，则查找并链接 FreeType 字体库
if(LV_USE_FREETYPE)
    find_package(Freetype REQUIRED)
    target_include_directories(lvgl PUBLIC ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(main ${FREETYPE_LIBRARIES})
endif()


# Windows 下 GUI 程序默认不显示控制台，此处保证日志输出可见
if (WIN32)
    if (MSVC)
        target_link_options(main PRIVATE "/SUBSYSTEM:CONSOLE")
    else()
        target_link_options(main PRIVATE "-mconsole")
    endif()
endif()


# 如果是 Debug 构建类型，添加额外的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode enabled") # 启用调试模式

    target_compile_options(lvgl PRIVATE
        -pedantic-errors
        -Wall
        -Wclobbered
        -Wdeprecated
        -Wdouble-promotion
        -Wempty-body
        -Wextra
        -Wformat-security
        -Wmaybe-uninitialized
        # -Wmissing-prototypes
        -Wpointer-arith
        -Wmultichar
        -Wno-pedantic # 暂时忽略，属性表中函数转指针
        -Wreturn-type
        -Wshadow
        -Wshift-negative-value
        -Wsizeof-pointer-memaccess
        -Wtype-limits
        -Wundef
        -Wuninitialized
        -Wunreachable-code
        -Wfloat-conversion
        -Wstrict-aliasing
    )

    # AddressSanitizer 内存检测工具相关配置
    if (ASAN)
        message(STATUS "AddressSanitizer enabled")

        # 添加 AddressSanitizer 编译和链接选项
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
    else()
        message(STATUS "AddressSanitizer disabled")
    endif()
endif()
