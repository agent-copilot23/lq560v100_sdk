#!/bin/bash

function usage ()
{
	echo "Usage: ${selfname}  load_script out_dir"
	echo "  load_script     full path of script to load ko. "
	echo "  out_dir         dir to install ko and load_script."
	echo ""
	echo "Example:"
	echo "  ${selfname} load_chipname  ompmod_dir"
	echo ""
	exit 0
}


selfname=$(basename $0)

if [ $# != 2 ]; then
	usage;
fi

script_path=$1
out_dir=$2
kos=`cat ${script_path} | grep "insmod .*\.ko"  | awk '{print $2}'`
ko_dir=$(dirname ${script_path})
mkdir -p ${out_dir}
for ko in ${kos}; do
	cp ${ko_dir}/${ko}  ${out_dir}/
done
cp ${script_path}  ${out_dir}/