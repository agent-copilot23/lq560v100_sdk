/*
  Copyright (c), 2021-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include "ot_partition_common.h"
#include "partition_env_tool_priv.h"

#define PARTTION_INFO_NODE_NAME     "Partition_Info"
#define PART_NODE_NAME              "Part"
#define ENV_NODE_NAME               "Env"

#define is_node(node, node_name) (strcmp((node)->name, node_name) == 0)

static const char *must_part[] = { NULL};

static int pack_str_field(char *dst, const char *src, unsigned long dst_len)
{
    int res = PT_FAILED;
    if (strlen(src) < dst_len) {
        strncpy(dst, src, dst_len);
        pt_trace("%s\n",src);
        res = PT_SUCCEESS;
    } else {
        pt_error(" [%s] is too long, %ld > %ld,\n", src, strlen(src), dst_len);
        res = PT_FAILED;
    }
    return res;
}

static int pack_spec_str_field(char *dst, const char *src, unsigned long min_len)
{
    int res = PT_FAILED;
    memcpy(dst, src, min_len);
    res = PT_SUCCEESS;
    return res;
}

static int pack_num_field(uint64_t *num, const char *src)
{
    int res = PT_FAILED;
    char * retptr= NULL;

    if ((NULL != src) && (NULL != num)) {
        uint64_t host_num = memparse(src, &retptr);
        *num = cpu_to_le64(host_num);
        pt_trace("host:%08lx packed:%08lx\n",host_num, *num);
    } else {
        pt_error("null number field\n");
    }
    /*no valid str or not end with empty*/
    if ((retptr == src) || ((retptr != NULL) && (*retptr != '\0')) ) {
        pt_error("invalid number, [%s]\n", src);
        res = PT_FAILED;
    } else {
        res = PT_SUCCEESS;
    }
    return res;
}

static const char *get_str(xmlNodePtr node, const char *field_name)
{

    xmlChar *res = NULL;
    if (xmlHasProp(node, field_name )) {
        res = xmlGetProp(node, field_name);
    }
    return (const char *)res;
}

static const char *get_env_str(xmlNodePtr node)
{

    xmlChar *res = NULL;
    res = xmlNodeGetContent(node);
    return (const char *)res;
}

static int pack_partition_info(xmlNodePtr node, ot_partition_info_t *packed_info)
{
    int res = PT_FAILED;
    xmlNodePtr partition_info = NULL;
    partition_info = node;
    if (NULL != partition_info) {
        const char *par_name = get_str(partition_info, "PartitionName");
        const char *fs_type = get_str(partition_info, "FileSystem");
        const char *start_addr = get_str(partition_info, "Start");
        const char *flash_type = get_str(partition_info, "FlashType");
        const char *length = get_str(partition_info, "Length");
        if ((NULL!=par_name) && (NULL!=flash_type) && (NULL!=start_addr)
            && (NULL!=flash_type) && (NULL!=length)) {

            if (PT_FAILED == pack_str_field(packed_info->partition_name, par_name, PARTITION_NAME_SZ)) {
                return res;
            }
            if (PT_FAILED == pack_str_field(packed_info->fs_type, fs_type, FS_TYPE_SZ)) {
                return res;
            }
            if (PT_FAILED == pack_str_field(packed_info->flash_type, flash_type, FLASH_TYPE_SZ)) {
                return res;
            }
            if (PT_FAILED == pack_num_field(&packed_info->length, length)) {
                return res;
            }
            if (PT_FAILED == pack_num_field(&packed_info->start_addr, start_addr)) {
                return res;
            }
            pt_trace("packed_info->start_addr:%lx\n", packed_info->start_addr);
        } else {
            pt_error("some field of this partition_info is empty\n");
            return res;
        }
    } else {
        pt_error("invalid array item\n");
        return res;
    }
    res = PT_SUCCEESS;
    return res;
}

static int save_env_info(xmlNodePtr node, const char *envpath)
{
    int res = PT_FAILED;
    xmlNodePtr env_node_info = NULL;
    env_node_info = node;
    if (NULL != env_node_info) {
        const char *env = get_env_str(env_node_info);
        if (NULL != env) {
            pt_trace("env: %s\n",env);
            save_env(envpath, env);
        } else {
            pt_error("some field of this env_node_info is empty\n");
            return res;
        }
    } else {
        pt_error("invalid array item\n");
        return res;
    }
    res = PT_SUCCEESS;
    return res;
}

static uint32_t get_array_size(xmlNodePtr node, const char *name)
{
    uint32_t size = 0;
    while (node != NULL) {
        if (strcmp(node->name, name) == 0) {
            size++;
        }
        node = node->next;
    }
    return size;
}

static int do_pack_block(ot_partition_block_info_t **block, uint8_t *payload, uint32_t payload_sz, int type)
{
    int res = -1;
    uint32_t block_header_sz = sizeof(ot_partition_block_info_t);
    void *pack = NULL;
    ot_partition_block_info_t *info = NULL;
    pt_trace("begin\n");
    if ((payload!=NULL) && (payload_sz != 0)) {
        pack = calloc(1, block_header_sz + payload_sz);
        memcpy(pack+block_header_sz, payload, payload_sz);
        info = (ot_partition_block_info_t *)pack;
        *block = info;
        info->block_size = cpu_to_le32(payload_sz);
        info->block_type = cpu_to_le32(type);
        res = 0;
    }
    pt_trace("end\n");
    return res;
}

static int do_pack_partitions(xmlNodePtr node, ot_partition_info_t **parts, const char *envpath, uint32_t *size)
{
    ot_partition_bmp_info_t *bmp_info = NULL;
    xmlNodePtr child = NULL;
    ot_partition_info_t *out_parts = NULL;
    uint32_t part_sz = sizeof(ot_partition_info_t);
    int res = -1;
    uint32_t array_sz = get_array_size(node->xmlChildrenNode, PART_NODE_NAME);
    if (array_sz > 0) {
        uint32_t part_info_sz = part_sz* array_sz; //array_sz应该-1，去掉最后一个part
        out_parts = (ot_partition_info_t *)calloc(1, part_info_sz);
        if (NULL == out_parts) {
            pt_error("no enough memory\n");
            res = -1;
            goto out;
        }

        int index = 0;
        child = node->xmlChildrenNode;
        pt_trace("array_sz:%d part_info_sz:%d\n", array_sz, part_info_sz);
        while(child != NULL) {
            pt_trace("par:%d\n", index);
            pt_trace("node_name:%s\n", child->name);
            if (is_node(child, PART_NODE_NAME)) {
                /*TOTO out_parts + index ???*/
                res = pack_partition_info(child, out_parts + index);
                if (res != 0) {
                    goto out;
                }
                index++;
            } else if(is_node(child, ENV_NODE_NAME)) {
                res = save_env_info(child, envpath);
                if (res != 0) {
                    goto out;
                }
                index++;
            }
            child = child->next;
        }
        res = 0;
        *parts = out_parts;
        *size = part_info_sz;
    } else {
        pt_error("empty partitions_info\n");
    }
out:
    if ((res != 0) && (out_parts != NULL)) {
        free((void *)out_parts);
    }
    return res;
}

static int do_must_check(ot_partition_info_t *parts,uint32_t size )
{
    int res = 0;
    uint32_t offset = 0;
    int i = 0;
    ot_partition_info_t *info = NULL;
    int has = 0;

    for (i = 0; must_part[i] != NULL;i++ ) {
        has = 0;
        for (offset = 0; offset < size; offset+=sizeof(ot_partition_info_t)) {
            info = (ot_partition_info_t *)((uint64_t)&parts[0] + offset);
            if (strcmp(must_part[i], info->partition_name) == 0) {
                pt_trace("got %s\n", must_part[i]);
                has = 1;
                break;
            }
        }
        if (has == 0) {
            pt_error("cfg SHOULD has %s\n", must_part[i]);
            res = -1;
            break;
        } else {
            res = 0;
        }
    }
    return res;
}

static int do_overlap_check(ot_partition_info_t *parts,uint32_t size )
{
    int res = -1;
    uint32_t offset = 0;
    ot_partition_info_t *info = NULL;
    ot_partition_info_t *next_info = NULL;
    uint64_t right_1st = 0;
    uint64_t left_2nd = 0;

    res = 0;
    for (offset = 0; offset < size - sizeof(ot_partition_info_t); offset+=sizeof(ot_partition_info_t)) {
        info = (ot_partition_info_t *)((uint64_t)&parts[0] + offset);
        next_info = (ot_partition_info_t *)((uint64_t)&parts[0] + offset + sizeof(ot_partition_info_t));
        right_1st = info->start_addr + info->length;
        left_2nd = next_info->start_addr;
        if (right_1st > left_2nd) {
            res = -1;
            pt_error("%s %s is overlap\n", info->partition_name, next_info->partition_name);
            break;
        }
    }
    return res;
}

static int do_verify_partitions(ot_partition_info_t *parts,uint32_t size )
{
    int res = -1;
    res = do_must_check(parts, size);
    if (res != 0) {
        return res;
    }
    res = do_overlap_check(parts, size);
    if (res != 0) {
        return res;
    }
    return 0;
}

static int pack_cfg(const char *fullpath, const char *envpath, ot_partition_bmp_info_t **bmp_info, uint32_t *bmp_size)
{
    xmlDocPtr doc;
    xmlNodePtr current;
    ot_partition_info_t *parts = NULL;
    ot_partition_block_info_t *parts_block = NULL;
    uint32_t parts_sz = 0;

    int res = -1;
    uint32_t bmp_header_sz = sizeof(ot_partition_bmp_info_t);
    uint32_t block_header_sz = sizeof(ot_partition_block_info_t);
    uint32_t bmp_sz = 0;
    void *pack = NULL;
    doc = xmlReadFile(fullpath, "UTF-8", XML_PARSE_RECOVER);
    if ((NULL != doc) && (NULL != (current = xmlDocGetRootElement(doc)))) {

        while (current != NULL) {
            pt_trace("node:%s\n", current->name);
            if (is_node(current, PARTTION_INFO_NODE_NAME)) {
                res = do_pack_partitions(current, &parts, envpath, &parts_sz);
                if (res == 0) {
                    res = do_verify_partitions(parts, parts_sz);
                    if (res != 0) {
                        res = -1;
                        goto out;
                    }
                } else {
                    res = -1;
                    goto out;
                }
            }
            current = current->next;
        }


        bmp_sz += bmp_header_sz;
        if (parts != NULL) {
            bmp_sz += block_header_sz + parts_sz;
            res = do_pack_block(&parts_block, (uint8_t *)parts, parts_sz, OT_PARTITION_BLOCK);
            if (res != 0) {
                res = -1;
                goto out;
            }
        }

        pack = calloc(1, bmp_sz);
        *bmp_info = (ot_partition_bmp_info_t *)pack;
        *bmp_size = bmp_sz;
        (*bmp_info)->bmp_info_size = cpu_to_le32(bmp_sz);
        memcpy(pack + bmp_header_sz, parts_block, parts_sz + block_header_sz);

        res = 0;
    } else {
        pt_error("invalid cfg file:%s\n", fullpath);
        res = -1;
    }
out:
    if (pack != NULL && (res != 0)) {
        free(pack);
    }
    if (parts != NULL) {
        free((void*)parts);
    }
    if (parts_block != NULL) {
        free((void*)parts_block);
    }

    return res;
}

int main(int argc, char **argv)
{
    ot_partition_bmp_info_t *bmp_info = NULL;
    uint32_t bmp_info_sz = 0;
    int res = -1;
    if (argc == 3 || argc == 4) {
        res = pack_cfg(argv[1], argv[3], &bmp_info, &bmp_info_sz);
        if (res == 0) {
            save_bin(argv[2], (uint8_t *)bmp_info, bmp_info_sz);
        } else {
            pt_error("pack %s failed\n", argv[1]);
            return -1;
        }
    } else {
        printf("usage:\n"
                "%s *.xml partition.bin env.txt\n", argv[0]);
    }
    return 0;
}
