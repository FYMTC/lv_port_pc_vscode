# main/ui/CMakeLists.txt

# 使用通配符自动添加所有源文件
file(GLOB_RECURSE UI_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# 排除不需要的文件（如果有的话）
# list(REMOVE_ITEM UI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/pre_page.cpp")

add_library(ui STATIC ${UI_SRC})

# 头文件包含路径
target_include_directories(ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/system
    ${CMAKE_CURRENT_SOURCE_DIR}/access
)

# 依赖 LVGL 和 LVGL examples
 target_link_libraries(ui PUBLIC lvgl lvgl::examples)
