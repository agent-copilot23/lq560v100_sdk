import os
import sys
import oem_main

SCRIPT_DIR = os.path.dirname(__file__)
SCRIPT_DIR = SCRIPT_DIR if len(SCRIPT_DIR) != 0 else '.'
WORK_DIR = os.path.abspath('%s/..' % SCRIPT_DIR)

os.chdir(WORK_DIR)
if len(sys.argv) == 2:
    oem_main.main(['', 'upgrade','oem/quick_build_config.json',sys.argv[1]])
else:
    print("error: Please afferent upgrade.zip",sys.argv[0])