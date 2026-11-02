一、切换芯片类型
1. 芯片默认为lq560plusv100
2. 切换为miniv100/litev100/tinyv100档位
    2.1 修改为lq560miniv100需修改 cfg.mak 文件
        将
            export CHIP = lq560plusv100
        替换为
            export CHIP = lq560miniv100
    2.2 修改为lq560litev100需修改 cfg.mak 文件
        将
            export CHIP = lq560plusv100
        替换为
            export CHIP = lq560litev100
    2.3 修改为lq560tinyv100需修改 cfg.mak 文件
        将
            export CHIP = lq560plusv100
        替换为
            export CHIP = lq560tinyv100

二、修改介质启动方式
1. 芯片默认为emmc启动方式
2. 从SPI启动，需修改 main/src/platform/Makefile 文件
   将
        BOOT_MEDIA ?= emmc
   替换为
        BOOT_MEDIA ?= spi

三、修改sensor型号
1. 芯片默认为 OS04A10 型号
2. 修改sensor型号（比如替换成OS08A20），需修改 main/src/component/media/sample/sample_cfg.mak 文件
   将
	SENSOR0_TYPE ?= OV_OS04A10_MIPI_4M_30FPS_12BIT
   替换为
	SENSOR0_TYPE ?= OV_OS08A20_MIPI_8M_30FPS_12BIT
   PS：如果有多个sensor需替换SENSOR0~SENSOR4
