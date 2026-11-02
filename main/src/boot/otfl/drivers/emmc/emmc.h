#ifndef __EMMC_H__
#define __EMMC_H__

/**
 * @brief 向eMMC指定位置写指定长度的数据
 *
 * @param buf 存储数据的buffer
 * @param offset 数据偏移，按照512对齐
 * @param length 数据长度
 * @return 成功：EXT_SUCCESS；失败：EXT_FAILURE
 */
int emmc_write(const unsigned int *buf, unsigned int offset, unsigned int length);

/**
 * @brief 从eMMC指定位置读指定长度的数据
 *
 * @param buf 存储数据的buffer
 * @param offset 数据偏移
 * @param length 数据长度
 * @return 成功：EXT_SUCCESS；失败：EXT_FAILURE
 */
int emmc_read(unsigned int *buf, unsigned int offset, unsigned int length);
#endif /* __EMMC_H__ */
