[toc]

# partition_env 使用说明

## 一、功能介绍
1、将xml文件解析成分区表bin文件(*.bin)和u-boot环境变量文本文件(*.env)

## 二、使用方法
### 2.1 生成xml文件解析工具
进入tool目录执行make all生成mk_partition_env解析工具

### 2.2 使用mk_partition_env将xml文件解析成对应的分区表bin文件和u-boot环境变量文本文件
```sh
./mk_partition_env 被解析的xml文件路径 分区表路径 env文本路径
```

### 2.3 将u-boot环境变量(env)从文本转换成可直接烧写的二进制镜像
执行 make 命令生成目标文件：
```sh
make ENVTXT=<env 文本路径> env

# 指定环境变量 SIZE（不指定 SIZE 默认 256KB）
make ENVTXT=<env 文本路径> SIZE=<size> env
```

在 partition_env/ 目录下生成 1 个目标文件：
- env.bin：二进制 U-Boot 环境变量，可直接烧写到介质

### 2.4 清除
分别在tool目录下和partition目录下执行 clean 清除生成的目标文件：
```sh
make clean
```

## 三、操作示例
以 config/lq560v100plus/nand.xml 为例：

### 3.1 生成xml解析工具
- 进入tool目录执行make all生成mk_partition_env
```sh
make all
```

### 3.2 生成分区表文件和env文本文件
```sh
./mk_partition_env ../config/lq560v100plus/nand.xml nand_partition.bin nand_env.txt
```

### 3.3 生成 env.bin
- 生成 size 为 256KB（默认）的 env.bin
```sh
make ENVTXT=tool/nand_env.txt env
```
- 生成 size 为 1MB 的 env.bin
```sh
make ENVTXT=tool/nand_env.txt SIZE=0x100000 env
```
