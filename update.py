import os
import shutil

src_dir = r'D:\esp32_code\lv_port_pc_vscode\main\ui'
dst_dir = r'D:\esp32_code\pthread\components\ui'
exts = ('.c', '.cpp', '.h')

# 1. 删除目标目录中源目录已删除的文件
for root, dirs, files in os.walk(dst_dir):
    rel_path = os.path.relpath(root, dst_dir)
    src_root = os.path.join(src_dir, rel_path)
    for file in files:
        if file.endswith(exts):
            src_file = os.path.join(src_root, file)
            dst_file = os.path.join(root, file)
            if not os.path.exists(src_file):
                os.remove(dst_file)

# 2. 复制源目录文件到目标目录（有重名自动覆盖）
for root, dirs, files in os.walk(src_dir):
    rel_path = os.path.relpath(root, src_dir)
    target_root = os.path.join(dst_dir, rel_path)
    os.makedirs(target_root, exist_ok=True)
    for file in files:
        if file.endswith(exts):
            src_file = os.path.join(root, file)
            dst_file = os.path.join(target_root, file)
            shutil.copy2(src_file, dst_file)
