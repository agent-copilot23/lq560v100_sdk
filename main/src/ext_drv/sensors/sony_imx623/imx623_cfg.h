#ifndef IMX623_CFG_H
#define IMX623_CFG_H

#define IMX623_CHIP_TYPE_ES3        // ES3 or ES2

#ifdef IMX623_CHIP_TYPE_ES3
#define IMX623_DATA_TYPE_RAW16      // RAW16 or RAW12
#define IMX623_FRAME_RATE_60FPS     // 60fps or 30 fps
#endif

#define IMX623_I2C_ADDR1            0x36 // multi sensor input, sensor1 i2c addr
#define IMX623_I2C_ADDR2            0x38 // multi sensor input, sensor2 i2c addr

#define MAX96717_I2C_ADDR           0x80 // serdes
#define MAX96712_I2C_ADDR           0x52 // deserdes
#define MAX9296_I2C_ADDR            0x90 // deserdes
#define IMX623_I2C_ADDR             0x34 // imx623 default i2c addr
#define IMX623_I2C_ADDR_I2C         0x36 // imx623 es2's i2c addr using i2c config sensor sequence
#define IMX623_I2C_ADDR_FLASH       0x34 // imx623 es2's i2c addr using flash config sensor sequence
#define MAX20087_I2C_ADDR           0x50 // control sensor & serdes power up

#endif /* IMX623_CFG_H */