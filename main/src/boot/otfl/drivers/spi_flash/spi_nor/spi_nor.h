#ifndef __SPI_NOR_H__
#define __SPI_NOR_H__

/**
 * @brief spi-nor flash 64KB block erase
 *
 * @param length 按照64KB擦除spi-nor flash
 * @param offset spi-nor flash偏移地址
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nor_erase(unsigned int length, unsigned int offset);

/**
 * @brief 向spi-nor flash指定位置写入指定长度的数据
 *
 * @param buf 存储数据的buffer
 * @param length 数据长度
 * @param offset spi-nor flash偏移地址
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nor_write(const unsigned char *buf, unsigned int length, unsigned int offset);

/**
 * @brief 从spi-nor flash指定位置读指定长度的数据
 *
 * @param buf 存储数据的buffer
 * @param length 数据长度
 * @param offset spi-nor flash偏移地址
 * @return 成功：EXT_SUCCESS; 失败：EXT_FAILURE
 */
int spi_nor_read(unsigned char* const buf, unsigned int length, unsigned int offset);

/**
 * @brief 初始化FMC，配置成normal mode
 *
 */
int spi_nor_init(void);

/**
 * @brief 去初始化FMC，恢复成boot mode
 *
 */
void spi_nor_deinit(void);
#endif /* __SPI_NOR_H__ */
