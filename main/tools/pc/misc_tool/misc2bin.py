#!/usr/bin/python3
# -*- coding:utf-8 -*-

import sys
import struct
import zlib

class LOG_LEVEL:
    DEBUG = 0
    INFO  = 1
    ERROR = 2

CURR_LEVEL = LOG_LEVEL.ERROR
#CURR_LEVEL = LOG_LEVEL.DEBUG
PLAT = None
QUICKBOOT_BUILD = False

def log_debug(debug_msg):
    if CURR_LEVEL > LOG_LEVEL.DEBUG:
        return
    print('[DEBUG] %s' % debug_msg)

def log_info(info_msg):
    if CURR_LEVEL > LOG_LEVEL.INFO:
        return
    print('[INFO] %s' % info_msg)

def log_error(error_msg):
    if CURR_LEVEL > LOG_LEVEL.ERROR:
        return
    print('[ERROR] %s' % error_msg)

def err_process(err_msg):
    log_error('%s (%s)' % (err_msg, ENV_FILE))
    sys.exit(1)


misc_version = "0010"

class misc_genrate:
    def __init__(self, magic0, magic1, last_boot):
        log_info("misc genrate version: " + misc_version);
        self.magic0 = magic0
        self.magic1 = magic1
        self.last_boot = last_boot
        self.total_slot = 2
        self.version = misc_version.encode(encoding="utf-8");
        self.reserved = 0
        self.crc32 = 0
        self.slot = [{}, {}];
        # only
        for i in range(self.total_slot):
            self.slot[i]["priority"] = 0;
            self.slot[i]["is_boot_ok"] = 0;
            self.slot[i]["retries_remain"] = 0;
            self.slot[i]["reserved"] = 0;

    def gen_slot(self, index, priority, is_boot_ok, retries_remain):
        if index >= self.total_slot:
            log_error("index error, index: %d" %(index));
            sys.exit(1);
        self.slot[index]["priority"] = priority;
        self.slot[index]["is_boot_ok"] = is_boot_ok;
        self.slot[index]["retries_remain"] = retries_remain;

    def __gen_slot_data(self, index):
        sf = struct.Struct("<4B");
        slot_data = sf.pack(self.slot[index]["priority"],
                self.slot[index]["is_boot_ok"],
                self.slot[index]["retries_remain"],
                self.slot[index]["reserved"],
                )
        return slot_data;

    def __gen_misc_data(self):
        mf = struct.Struct("<I")
        misc_data = mf.pack(self.magic0);
        misc_data += mf.pack(self.magic1);
        #  misc_data += mf.pack(self.version);
        misc_data += self.version;
        misc_data += self.slot0_data + self.slot1_data;

        mf = struct.Struct("<2I")
        misc_data += mf.pack(self.last_boot, self.reserved);
        self.crc32 = zlib.crc32(misc_data);
        mf = struct.Struct("<I");
        misc_data += mf.pack(self.crc32);
        return misc_data;

    def out_misc_file(self, out_name, block_num, block_size, block_slots, page_size):
        self.slot0_data = self.__gen_slot_data(0);
        self.slot1_data = self.__gen_slot_data(1);
        misc_data = self.__gen_misc_data();

        #fill block:
        pad_byte = b'\xff';
        page_data = misc_data + (page_size - len(misc_data)) * pad_byte;
        page_data *= block_slots;
        block_data = page_data;
        if len(page_data) < block_size:
            block_data += (block_size - len(page_data))  * pad_byte;

        out_data = block_data * block_num;
        with open(out_name, 'wb') as f:
            f.write(out_data);
        return;

def print_usage():
    print('Usage:')
    print('python3 %s <output_file_name>' % sys.argv[0])


if __name__=="__main__":
    if (len(sys.argv) < 2):
        print_usage();
        sys.exit(1);
    magic_0 = 0x42414342;
    magic_1 = 0x42414342;
    last_boot = 0xABABABAB; #invaild num
    misc = misc_genrate(magic_0, magic_1, last_boot);

    #slot A:
    slot_index = 0;
    slot_priority = 15;
    slot_is_book_ok = 1;
    slot_retries_remain = 1;
    misc.gen_slot(slot_index, slot_priority, slot_is_book_ok, slot_retries_remain);

    #slot B:
    slot_index = 1;
    slot_priority = 14;
    slot_is_book_ok = 1;
    slot_retries_remain = 1;
    misc.gen_slot(slot_index, slot_priority, slot_is_book_ok, slot_retries_remain);

    block_num = 2;
    block_size = 256*1024;
    block_slots = 2;
    page_size = 4*1024;
    misc.out_misc_file(sys.argv[1], block_num, block_size, block_slots, page_size);


