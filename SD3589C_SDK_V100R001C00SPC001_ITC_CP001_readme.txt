补丁名称：SD3589C_SDK_V100R001C00SPC001_ITC_CP001

补丁作用范围：ICT开发专用补丁

补丁具体描述
1. 支持UCV 功能


补丁发布日期：2026/10/10

补丁依赖关系：[无依赖关系]

补丁使用说明：
将补丁文件放在sdk目录，执行命令
patch -p1 < SD3589C_SDK_V100R001C00SPC001_ITC_CP001.patch
后执行./build.sh命令进行编译，uvc app 路径为：lq560v100.custom/main/src/component/media/sample/uvc_app
