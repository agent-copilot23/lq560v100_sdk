#!/bin/sh
########  header formet ###############
# [ compress size ] [   orig size   ] [  magic string ]
# [     4Bytes    ] [     4Bytes    ] [     8Bytes    ]
# [  header2.bin  ] [  header1.bin  ] [  header3.bin  ]
pid=$$
InputImage=$1
if [ $InputImage ]; then
    size=$(wc -c $InputImage | awk '{print $1}')
    size=$(echo $size | xargs printf "%08x")
    size=$(echo ${size:6:2}${size:4:2}${size:2:2}${size:0:2})
    echo $size | xxd -r -ps > ${pid}_header1.bin

    ${OPENSOURCE_DIR}/gzip/bin/gzip -f ${InputImage}

    size=$(wc -c ${InputImage}.gz | awk '{print $1}')
    size=$(echo $size | xargs printf "%08x")
    size=$(echo ${size:6:2}${size:4:2}${size:2:2}${size:0:2})
    echo $size | xxd -r -ps > ${pid}_header2.bin

    echo -n "gziphead" | xxd -p | xxd -r -ps > ${pid}_header3.bin

    mv ${InputImage}.gz ${InputImage}.tmp.gz

    cat ${pid}_header2.bin ${pid}_header1.bin ${pid}_header3.bin ${InputImage}.tmp.gz > ${InputImage}.gz

    rm ${pid}_header1.bin ${pid}_header2.bin ${pid}_header3.bin ${InputImage}.tmp.gz
else
    echo "Usage: ./kernel_compress.sh <imageName>"
    echo "Example: ./kernel_compress.sh uImage"
fi