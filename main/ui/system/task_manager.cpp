/**
 * @file task_manager.cpp
 * @brief ESP32任务管理器模拟实现，提供空的接口实现用于编译
 *
 * 功能：
 * - 模拟接口实现，所有方法返回默认值
 * - 用于在PC环境下编译通过
 *
 * @author
 * @date 2025-07-30
 */

#include "task_manager.hpp"
#include <iostream>

static const char* TAG = "TaskManager";

TaskManager& TaskManager::instance() {
    static TaskManager instance;
    return instance;
}

std::vector<TaskManager::TaskInfo> TaskManager::get_all_tasks_info() const {
    // 返回模拟的任务信息
    std::vector<TaskInfo> tasks_info;

    // 添加一些模拟任务
    TaskInfo main_task;
    main_task.name = "main";
    main_task.priority = 5;
    main_task.stack_high_water_mark = 4096;
    main_task.core_id = 0;
    main_task.state = static_cast<eTaskState>(1); // eReady
    main_task.runtime = 1000;
    tasks_info.push_back(main_task);

    TaskInfo ui_task;
    ui_task.name = "ui_task";
    ui_task.priority = 3;
    ui_task.stack_high_water_mark = 8192;
    ui_task.core_id = 1;
    ui_task.state = static_cast<eTaskState>(2); // eBlocked
    ui_task.runtime = 500;
    tasks_info.push_back(ui_task);

    return tasks_info;
}

TaskManager::SystemInfo TaskManager::get_system_info() const {
    // 返回模拟的系统信息
    return {
        .free_heap = 200 * 1024,      // 200KB 可用内存
        .min_free_heap = 100 * 1024,  // 100KB 最小可用内存
        .total_allocated = 512 * 1024, // 512KB 总分配内存
        .cpu_usage = 25               // 25% CPU使用率
    };
}

void TaskManager::print_top_like_output() const {
    std::cout << "\n\nPC Simulator Task Manager (Mock)\n";
    std::cout << "============================\n";
    std::cout << "System Time: Mock Time\n";
    std::cout << "Memory: Free=200 kbytes, Min Free=100 kbytes, Total=512 kbytes\n";
    std::cout << "\n";
    std::cout << "Task Name           Prio    Core    State       Stack       CPU Usage   Killable\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "main                5       0       Ready       4096        50%         N\n";
    std::cout << "ui_task             3       1       Blocked     8192        25%         Y\n";
}

TaskManager::KillResult TaskManager::kill_task(const std::string& task_name) {
    std::cout << "Mock: Attempting to kill task: " << task_name << std::endl;

    if (task_name == "main" || task_name == "IDLE") {
        return KillResult::TASK_IS_CRITICAL;
    }

    if (task_name == "unknown_task") {
        return KillResult::TASK_NOT_FOUND;
    }

    std::cout << "Mock: Task " << task_name << " killed successfully" << std::endl;
    return KillResult::SUCCESS;
}

TaskManager::KillResult TaskManager::kill_task(TaskHandle_t handle) {
    std::cout << "Mock: Attempting to kill task by handle" << std::endl;

    if (handle == nullptr) {
        return KillResult::TASK_NOT_FOUND;
    }

    std::cout << "Mock: Task killed successfully" << std::endl;
    return KillResult::SUCCESS;
}

void TaskManager::task_suicide() {
    std::cout << "Mock: Current task is committing suicide" << std::endl;
    // 在模拟环境中不实际删除任务
}

bool TaskManager::is_task_killable(TaskHandle_t handle) const {
    // 模拟实现，假设大部分任务都是可以终止的
    return handle != nullptr;
}

TaskManager::TaskInfo TaskManager::get_task_info(TaskHandle_t handle) const {
    TaskInfo info;
    if (handle != nullptr) {
        info.name = "mock_task";
        info.priority = 3;
        info.stack_high_water_mark = 4096;
        info.core_id = 0;
        info.state = static_cast<eTaskState>(1); // eReady
        info.runtime = 100;
    }
    return info;
}
