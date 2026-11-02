配置文件 imx623_cfg.h 说明:

1. 芯片类型选择
    IMX623_CHIP_TYPE_ES3
    IMX623 支持 ES3 和 ES2, 上述宏被定义时为 ES3, 否则为 ES2

2. 数据类型选择
    IMX623_DATA_TYPE_RAW16
    IMX623 为 ES3 时可选项, 宏被定义时为 RAW16, 否则为 RAW12

3. 帧率选择
    IMX623_FRAME_RATE_60FPS
    IMX623 为 ES3 时可选项, 宏被定义时为 60fps, 否则为 30fps
