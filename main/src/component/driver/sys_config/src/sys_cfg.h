/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef SYS_CFG_H
#define SYS_CFG_H

#include <linux/printk.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define sys_config_print(format, args...) \
    printk("%s,%d: "format, __FUNCTION__, __LINE__, ##args) \

#define sys_writel(addr, value) ((*((volatile unsigned int *)(addr))) = (value))
#define sys_read(addr) (*((volatile int *)(addr)))
void write_reg32(unsigned long addr, unsigned long value, unsigned long mask);

void *sys_config_get_reg_crg(void);
void *sys_config_get_reg_sys(void);
void *sys_config_get_reg_ddr(void);
void *sys_config_get_reg_misc(void);
void *sys_config_get_reg_iocfg(void);
void *sys_config_get_reg_iocfg2(void);
void *sys_config_get_reg_iocfg3(void);
void *sys_config_get_reg_iocfg4(void);
void *sys_config_get_reg_gpio(void);
void *sys_config_get_reg_mipi_tx(void);

#define CHIP_LQ560V100PLUS 0x0
#define CHIP_NAME_LQ560V100PLUS "lq560v100plus"
#define VO_INTF_NAME_MIPI_TX "mipi_tx"
#define IR_AUTO_ENABLE  "1"

#define BOARD_SCK       0x0
#define BOARD_NAME_SCK      "sck"
#define BOARD_DMEB   0x1
#define BOARD_NAME_DMEB "dmeb"
#define BOARD_DMEBPRO   0x2
#define BOARD_NAME_DMEBPRO "dmebpro"

bool chip_is_lq560v100tiny(void);
int sys_config_get_main_board_type(void);
int sys_config_get_chip_type(void);
bool sys_config_get_mipitx_swi(void);
bool sys_config_get_quickboot_en(void);
int sys_config_get_board_type(void);
int sys_config_get_vi_vpss_mode(void);
int sys_config_get_vo_intf_type(void);
int sys_config_get_vi_intf_type(void);
int sys_config_get_ir_auto(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SYS_CFG_H */
