user_mount_dir=/opt
export LD_LIBRARY_PATH=$user_mount_dir/lib/:$LD_LIBRARY_PATH
export PATH=$PATH:/opt/bin
mmc_mount_device=/dev/mmcblk1
function waitfor
{
    cnt=0
    path=$1
    to=$2
    while [ 1 ];do
        if [ ! -e "$path" ];then
            sleep 0.1
        else
            break;
        fi
        let cnt=$cnt+100
        if [ $cnt -gt $to ];then
            echo "waitfor $path timeout($to)"
            break;
        fi
    done
}

if [ -e $user_mount_dir/ompmod/load_lq560v100 ];then
    cd $user_mount_dir/ompmod/
    ./load_lq560v100 -i
fi

if [ -e $user_mount_dir/ompmod/load_lq560v100_quickboot_stage_2 ];then
    cd $user_mount_dir/ompmod/
    ./load_lq560v100_quickboot_stage_2 -i
fi

if [ -e $user_mount_dir/kermod/load_kermod.sh ];then
    cd $user_mount_dir/kermod/
    ./load_kermod.sh
fi

waitfor $mmc_mount_device 1000
mkdir -p /tmp/emmc/
if [ -e $mmc_mount_device ];then
    mount $mmc_mount_device /tmp/emmc/
    if [ $? != 0 ];then
        mkfs.ext4 -F $mmc_mount_device
        mount $mmc_mount_device /tmp/emmc/
    fi
else
    mkdir -p /data/upgrade
    ln -s /data/upgrade /tmp/emmc/upgrade
fi

if [ -e $user_mount_dir/bin/sample_upgrade ];then
    cd $user_mount_dir/bin/
    ##sleep 3 to avoid compete for CPU usage with quickboot
    sleep 3 && ./sample_upgrade &
fi