#ifndef __SPI_NAND_H__
#define __SPI_NAND_H__

/**
 * @brief 擦除spi-nand flash指定位置(eraseblock)、指定长度(length)的块
 *
 * @param eraseblock 擦除块偏移, 按照128KB/256KB对齐, 比如0, 0x20000, 0x40000...
 * @param length 擦除长度，按照128KB/256KB对齐，, 比如0x20000, 0x40000...
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nand_erase(unsigned int eraseblock, unsigned int length);

/**
 * @brief 向spi-nand flash指定位置(offset)写入指定长度(length)的数据
 *
 * @param buf 数据
 * @param length 数据长度，不需要按照pagesize对齐
 * @param offset 数据偏移，按照pagesize对齐(2K/4k)
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nand_write(const unsigned char *buf, unsigned int length, unsigned int offset);

/**
 * @brief 从spi-nand flash指定位置(offset)读指定长度(length)的数据
 *
 * @param buf 数据
 * @param length 数据长度
 * @param offset 数据偏移
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nand_read(unsigned char *buf, unsigned int length, unsigned int offset);

/**
 * @brief 判断spi-nand flash的块是否为坏块
 *
 * @param offset spi-nand flash地址偏移，按照blocksize对齐
 * @return TRUE：坏块
 * @return FALSE：不是坏块
 */
bool is_bad_block(unsigned int offset);
/**
 * @brief FMC设置成normal模式，初始化pagesize/blocksize/DMA buffer
 *
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nand_init(void);

/**
 * @brief FMC恢复成boot模式
 *
 */
void spi_nand_deinit(void);

/**
 * @brief 获取spi-nand flash的pagesize
 *
 * @param page_size ：保存spi-nand flash的pagesize(2K/4K)
 */
void spi_nand_page_size(unsigned int *page_size);

/**
 * @brief 获取spi-nand flash的blocksize
 *
 * @param block_size ：保存spi-nand flash的block_size(128K/256K)
 */
void spi_nand_block_size(unsigned int *block_size);

/**
 * @brief 判断是spi-nand flash还是spi-nor flash
 *
 * @return true  : spi-nand flash
 * @return false : spi-nor flash
 */
bool is_nand(void);
#endif /* __SPI_NAND_H__ */
