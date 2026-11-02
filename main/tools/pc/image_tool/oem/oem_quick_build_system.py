import os
import sys
import oem_main

SCRIPT_DIR = os.path.dirname(__file__)
SCRIPT_DIR = SCRIPT_DIR if len(SCRIPT_DIR) != 0 else '.'
WORK_DIR = os.path.abspath('%s/..' % SCRIPT_DIR)

os.chdir(WORK_DIR)
if len(sys.argv) == 3:
    oem_main.main(['', 'system','oem/quick_build_config.json', sys.argv[1], sys.argv[2]])
else:
    print("error: Please input system image file and output file",sys.argv[0])
