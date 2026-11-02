一. json 文件转为 .h 使用说明：
1. 在 对应的 json 文件目录添加 配置文件 config
2. 在 json 文件目录下运行 python  程序 json_to_h

二. 举例(imx623)：
1. 在 json 文件目录：data/imx623  添加  config
2. 运行 json_to_h ： python ../json_to_h.py

三. 配置文件 config.ini：
参考 data/imx623/config.ini , Config.ini 文件有两种类型配置
1. sensor_awb_cfg.json,imx623_sensor_awb.h,imx623
    1) sensor_awb_cfg.json 对应的json文件名
    2) imx623_sensor_awb.h 生成的头文件名
    3) imx623 为定义变量前缀
2. sensor_table_cfg.json,imx623_sensor_table.h,imx623,2
    1) sensor_table_cfg.json 对应的json文件名
    2) imx623_sensor_table.h 生成的头文件名
    3) imx623 为定义变量前缀
    4) 2 为每两个变量组成一个元组
