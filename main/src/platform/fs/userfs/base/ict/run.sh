export UVC_DEVICE_CNT=1
export BCDUVC=0x0110
export VID="0x3ef5"
export PID="0x0ef0"
export MANUFACTURED="Oritek"
export PRODUCT="UVC"
export SERIALNUMBER=$(head -c 12 /dev/mtdblock4)

export YUYV="360p 720p 3000p 6000p"
export MJPEG="720p 1080p 1440p 2160p 3000p 6000p"

export TransferMode="bulk"
export PerfMode="v4l2"

#2026.10.22  修改，增加曝光接口
#export CamControl1=0x0A
export CamControl1=0x2A
export CamControl2=0xAA
export CamControl3=0x00
#2026.10.22  修改，增加曝光接口
#export ProcControl1=0xFF
export ProcControl1=0x7F
export ProcControl2=0x17
export ProcControl3=0x00
export EcdControl1=0x00
export EcdControl2=0x00
export EcdControl3=0x00
export EcdRtControl1=0x00
export EcdRtControl2=0x00
export EcdRtControl3=0x00
export ExtControl1=0x00
export ExtControl2=0x00

echo device > /proc/10320000.usb30drd/mode
sleep 0.5

sh ./usb-uvc.sh
sleep 0.5

./uvc_app &

