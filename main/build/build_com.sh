#!/bin/sh


# common options
quickboot=no
secureboot=no

# shell val
com_shortopt="qhs"
com_longopt="quickboot,help,secureboot"

function help_com()
{
    selfname="./build.sh"
    printf "%s\n" "
    usage : $selfname <options>
    options : only support one of below.
        [-h|--help]                 //help message
        [-q|--quickboot]            //enable quickboot
        [-s|--secureboot]           //enable secureboot"
}
function help_com_example()
{
    printf "%s\n" "
    example:
        $selfname -q             //enable quickboot and build
        $selfname --quickboot    //enable quickboot and build
        $selfname -s             //enable secureboot and build
        $selfname --seucreboot   //enable secureboot and build"
}

function set_com_var()
{
    export QUICKBOOT_BUILD="$quickboot"
    export SECUREBOOT_BUILD="$secureboot"
    printf "%s\n" "
        ------- common config ------
          quickboot:$quickboot
          secureboot:$secureboot
        ----------------------------
    "
}

# 第一个参数为short_opt 第二个参数集为long_opt 第三之后的为要解析的参数集
function parse_com_var()
{
    ret=0;
    echo "------------ com_parse:$@ ------------"
    local opt1=$1;
    local opt2=$2;
    shift 2;

    TEMP=$(getopt -o $opt1 --long $opt2  -- "$@")

    eval set -- "$TEMP"

    while true; do
        #echo "com_debug $1"
        case "$1" in
            -h|--help) ret=1; return; shift;;         #正常退出打印help
            -q|--quickboot) quickboot="y"; shift;;
            -s|--seucreboot) secureboot="y"; shift;;
            --) shift; break;;
            *) echo "Internal error!"; return;;
        esac
    done
}
