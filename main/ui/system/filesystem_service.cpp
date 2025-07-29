#include "filesystem_service.hpp"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#include <direct.h>
#include <io.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#endif

static bool filesystem_initialized = false;

esp_err_t fs_init(void) {
    filesystem_initialized = true;
    return ESP_OK;
}void filesystem_service_deinit(void) {
    filesystem_initialized = false;
}

bool filesystem_service_is_available(void) {
    return filesystem_initialized;
}

#ifdef _WIN32
// Windows特定的文件时间转换函数
static time_t filetime_to_time_t(const FILETIME *ft) {
    ULARGE_INTEGER ui;
    ui.LowPart = ft->dwLowDateTime;
    ui.HighPart = ft->dwHighDateTime;
    return (time_t)((ui.QuadPart - 116444736000000000ULL) / 10000000ULL);
}
#endif

// 实际的路径存在检查
bool fs_is_path_exists(const char *path) {
    if (!filesystem_initialized || !path) return false;

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(path);
    return (attrs != INVALID_FILE_ATTRIBUTES);
#else
    struct stat st;
    return (stat(path, &st) == 0);
#endif
}

// 实际的目录检查
bool fs_is_directory(const char *path) {
    if (!filesystem_initialized || !path) return false;

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(path);
    return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    return (stat(path, &st) == 0) && S_ISDIR(st.st_mode);
#endif
}

// 实际的文件检查
bool fs_is_file(const char *path) {
    if (!filesystem_initialized || !path) return false;

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(path);
    return (attrs != INVALID_FILE_ATTRIBUTES) && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    return (stat(path, &st) == 0) && S_ISREG(st.st_mode);
#endif
}

esp_err_t fs_remove_directory(const char *path) {
    if (!filesystem_initialized || !path) return ESP_ERR_INVALID_ARG;

#ifdef _WIN32
    return RemoveDirectoryA(path) ? ESP_OK : ESP_FAIL;
#else
    return rmdir(path) == 0 ? ESP_OK : ESP_FAIL;
#endif
}

esp_err_t fs_list_directory(const char* path, file_info_t **files, int *count, sort_type_t sort) {
    if (!filesystem_initialized || !path || !files || !count) {
        return ESP_ERR_INVALID_ARG;
    }

    *files = nullptr;
    *count = 0;

    // 临时存储文件列表
    std::vector<file_info_t> file_list;

#ifdef _WIN32
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle;

    // 构建搜索模式
    char search_pattern[MAX_PATH_LEN];
    snprintf(search_pattern, sizeof(search_pattern), "%s\\*", path);

    find_handle = FindFirstFileA(search_pattern, &find_data);
    if (find_handle == INVALID_HANDLE_VALUE) {
        return ESP_ERR_NOT_FOUND;
    }

    do {
        // 跳过当前目录和父目录引用
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) {
            continue;
        }

        file_info_t file_info = {};

        // 设置文件名
        strncpy(file_info.name, find_data.cFileName, sizeof(file_info.name) - 1);

        // 设置完整路径
        snprintf(file_info.full_path, sizeof(file_info.full_path), "%s\\%s", path, find_data.cFileName);

        // 设置文件类型
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            file_info.type = FILE_TYPE_DIRECTORY;
            file_info.size = 0;
        } else {
            file_info.type = FILE_TYPE_REGULAR;
            LARGE_INTEGER file_size;
            file_size.LowPart = find_data.nFileSizeLow;
            file_size.HighPart = find_data.nFileSizeHigh;
            file_info.size = file_size.QuadPart;
        }

        // 设置修改时间
        file_info.modified_time = filetime_to_time_t(&find_data.ftLastWriteTime);

        // 设置隐藏属性
        file_info.is_hidden = (find_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;

        file_list.push_back(file_info);

    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);

#else // Linux/Unix实现
    DIR *dir = opendir(path);
    if (!dir) {
        return ESP_ERR_NOT_FOUND;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        // 跳过当前目录和父目录引用
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        file_info_t file_info = {};

        // 设置文件名
        strncpy(file_info.name, entry->d_name, sizeof(file_info.name) - 1);

        // 设置完整路径
        snprintf(file_info.full_path, sizeof(file_info.full_path), "%s/%s", path, entry->d_name);

        // 获取文件详细信息
        struct stat file_stat;
        if (stat(file_info.full_path, &file_stat) == 0) {
            if (S_ISDIR(file_stat.st_mode)) {
                file_info.type = FILE_TYPE_DIRECTORY;
                file_info.size = 0;
            } else {
                file_info.type = FILE_TYPE_REGULAR;
                file_info.size = file_stat.st_size;
            }
            file_info.modified_time = file_stat.st_mtime;
        } else {
            file_info.type = FILE_TYPE_UNKNOWN;
            file_info.size = 0;
            file_info.modified_time = 0;
        }

        // 设置隐藏属性（Unix系统中以.开头的文件被认为是隐藏的）
        file_info.is_hidden = (entry->d_name[0] == '.');

        file_list.push_back(file_info);
    }

    closedir(dir);
#endif

    // 排序文件列表
    switch (sort) {
        case SORT_BY_NAME_ASC:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    return strcmp(a.name, b.name) < 0;
                });
            break;
        case SORT_BY_NAME_DESC:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    return strcmp(a.name, b.name) > 0;
                });
            break;
        case SORT_BY_SIZE_ASC:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    return a.size < b.size;
                });
            break;
        case SORT_BY_SIZE_DESC:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    return a.size > b.size;
                });
            break;
        case SORT_BY_TIME_ASC:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    return a.modified_time < b.modified_time;
                });
            break;
        case SORT_BY_TIME_DESC:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    return a.modified_time > b.modified_time;
                });
            break;
        case SORT_BY_TYPE:
            std::sort(file_list.begin(), file_list.end(),
                [](const file_info_t &a, const file_info_t &b) {
                    if (a.type != b.type) {
                        return a.type < b.type; // 目录优先
                    }
                    return strcmp(a.name, b.name) < 0;
                });
            break;
    }

    // 分配内存并复制结果
    *count = file_list.size();
    if (*count > 0) {
        *files = (file_info_t*)malloc(*count * sizeof(file_info_t));
        if (!*files) {
            return ESP_ERR_NO_MEM;
        }

        for (int i = 0; i < *count; i++) {
            (*files)[i] = file_list[i];
        }
    }

    return ESP_OK;
}

void fs_free_file_list(file_info_t *files, int count) {
    if (files) {
        free(files);
    }
}

esp_err_t fs_open_directory(const char *path, dir_iterator_t *iterator) {
    if (!iterator) return ESP_ERR_INVALID_ARG;
    iterator->dir_handle = nullptr;
    strcpy(iterator->current_path, path ? path : "/sd");
    iterator->file_count = 0;
    iterator->dir_count = 0;
    return ESP_OK;
}

esp_err_t fs_read_next_file(dir_iterator_t *iterator, file_info_t *file_info) {
    return ESP_ERR_NOT_FOUND; // 模拟没有更多文件
}

void fs_close_directory(dir_iterator_t *iterator) {
    if (iterator) {
        iterator->dir_handle = nullptr;
    }
}

esp_err_t fs_copy_file(const char *src, const char *dst) {
    return ESP_OK;
}

esp_err_t fs_move_file(const char *src, const char *dst) {
    return ESP_OK;
}

esp_err_t fs_rename_file(const char *old_name, const char *new_name) {
    return ESP_OK;
}

esp_err_t fs_get_parent_path(const char *path, char *parent_path, size_t parent_path_size) {
    if (!path || !parent_path || parent_path_size == 0) return ESP_ERR_INVALID_ARG;

    // 复制路径
    strncpy(parent_path, path, parent_path_size - 1);
    parent_path[parent_path_size - 1] = '\0';

    // 移除末尾的路径分隔符
    size_t len = strlen(parent_path);
    while (len > 0 && (parent_path[len - 1] == '\\' || parent_path[len - 1] == '/')) {
        parent_path[len - 1] = '\0';
        len--;
    }

    // 找到最后一个路径分隔符
    char *last_separator = nullptr;
    for (int i = len - 1; i >= 0; i--) {
        if (parent_path[i] == '\\' || parent_path[i] == '/') {
            last_separator = &parent_path[i];
            break;
        }
    }

    if (last_separator) {
        *last_separator = '\0';
        // 如果结果为空或只是驱动器号，保留根目录
        if (strlen(parent_path) == 0) {
#ifdef _WIN32
            strcpy(parent_path, "C:");
#else
            strcpy(parent_path, "/");
#endif
        }
#ifdef _WIN32
        else if (strlen(parent_path) == 2 && parent_path[1] == ':') {
            strcat(parent_path, "\\");
        }
#endif
    } else {
        // 没有找到分隔符，返回根目录
#ifdef _WIN32
        strcpy(parent_path, "C:\\");
#else
        strcpy(parent_path, "/");
#endif
    }

    return ESP_OK;
}

esp_err_t fs_join_path(const char *base_path, const char *sub_path, char *result_path, size_t result_size) {
    if (!base_path || !sub_path || !result_path) return ESP_ERR_INVALID_ARG;

#ifdef _WIN32
    const char separator = '\\';
#else
    const char separator = '/';
#endif

    // 检查base_path是否以分隔符结尾
    size_t base_len = strlen(base_path);
    bool need_separator = (base_len > 0 && base_path[base_len - 1] != '\\' && base_path[base_len - 1] != '/');

    if (need_separator) {
        snprintf(result_path, result_size, "%s%c%s", base_path, separator, sub_path);
    } else {
        snprintf(result_path, result_size, "%s%s", base_path, sub_path);
    }

    return ESP_OK;
}

esp_err_t fs_get_filename(const char *path, char *filename, size_t filename_size) {
    if (!path || !filename) return ESP_ERR_INVALID_ARG;

    const char *last_separator = nullptr;
    for (const char *p = path; *p; p++) {
        if (*p == '\\' || *p == '/') {
            last_separator = p;
        }
    }

    const char *name = last_separator ? last_separator + 1 : path;
    strncpy(filename, name, filename_size - 1);
    filename[filename_size - 1] = '\0';
    return ESP_OK;
}

esp_err_t fs_get_file_extension(const char *path, char *extension, size_t extension_size) {
    if (!path || !extension) return ESP_ERR_INVALID_ARG;

    const char *dot = strrchr(path, '.');
    if (dot && dot != path) {
        strncpy(extension, dot + 1, extension_size - 1);
        extension[extension_size - 1] = '\0';
    } else {
        extension[0] = '\0';
    }
    return ESP_OK;
}

// 实现存储信息函数
esp_err_t fs_get_storage_info(const char *path, storage_info_t *info) {
    if (!info) return ESP_ERR_INVALID_ARG;

#ifdef _WIN32
    // 获取驱动器盘符
    char root_path[4] = "C:\\";
    if (path && strlen(path) >= 2 && path[1] == ':') {
        root_path[0] = path[0];
    }

    ULARGE_INTEGER free_bytes, total_bytes, total_free_bytes;
    if (GetDiskFreeSpaceExA(root_path, &free_bytes, &total_bytes, &total_free_bytes)) {
        info->total_bytes = total_bytes.QuadPart;
        info->free_bytes = free_bytes.QuadPart;
        info->used_bytes = info->total_bytes - info->free_bytes;
        info->is_mounted = true;

        // 获取文件系统类型
        char volume_name[256];
        char filesystem_type[32];
        DWORD serial_number, max_component_len, filesystem_flags;

        if (GetVolumeInformationA(root_path, volume_name, sizeof(volume_name),
                                  &serial_number, &max_component_len, &filesystem_flags,
                                  filesystem_type, sizeof(filesystem_type))) {
            strncpy(info->filesystem_type, filesystem_type, sizeof(info->filesystem_type) - 1);
            info->filesystem_type[sizeof(info->filesystem_type) - 1] = '\0';
        } else {
            strcpy(info->filesystem_type, "NTFS");
        }
    } else {
        return ESP_FAIL;
    }
#else
    // Linux/Unix 实现
    struct statvfs st;
    if (statvfs(path ? path : "/", &st) == 0) {
        info->total_bytes = (uint64_t)st.f_blocks * st.f_frsize;
        info->free_bytes = (uint64_t)st.f_bavail * st.f_frsize;
        info->used_bytes = info->total_bytes - info->free_bytes;
        info->is_mounted = true;
        strcpy(info->filesystem_type, "ext4");
    } else {
        return ESP_FAIL;
    }
#endif

    return ESP_OK;
}
