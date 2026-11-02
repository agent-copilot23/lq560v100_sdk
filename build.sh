#!/bin/sh

CUR_DIR=$(cd $(dirname $0); pwd); 

# 解析参数，并配置环境变量
source ${CUR_DIR}/main/build/build_com.sh
function help()
{
    help_com $@;
    help_com_example;
}
function build()
{
    set_com_var
    make all -j16
}

# parse shell option & build
parse_com_var $com_shortopt $com_longopt $@

if [ $ret = 1 ]; then
    help;
    exit 0;
else
    build;
fi
