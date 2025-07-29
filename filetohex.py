#!/usr/bin/env python3
import sys
import os
import textwrap
import re

input_path = sys.argv[1]
filename = os.path.basename(input_path)
name, ext = os.path.splitext(filename)
output_path = os.path.join(os.path.dirname(input_path), name + '.c')

with open(input_path, 'r') as file:
    s = file.read()

b = bytearray()

if '--filter-character' in sys.argv:
    s = re.sub(r'[^\x00-\x7f]', '', s)
if '--null-terminate' in sys.argv:
    s += '\x00'

b.extend(map(ord, s))

array_name = name
c_code = f'#include "lvgl.h"\n\nconst uint8_t {array_name}[] = {{\n    {textwrap.fill(", ".join([hex(a) for a in b]), 96)}\n}};\nconst size_t {array_name}_len = sizeof({array_name});\n'

with open(output_path, 'w') as out:
    out.write(c_code)
