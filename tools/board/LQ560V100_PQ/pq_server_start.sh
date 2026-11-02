#!/bin/sh

timeout=0
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/libs
killall pq_server
while true;do
    CtrlPID=$(pidof pq_server)
    if [ -z "$CtrlPID" ];then
        echo    ">>>>no pid,run it"
        break
    else
	    echo "control PID:" $CtrlPID
        echo ">>>>program is exiting waiting"
        sleep 2
        timeout=$(expr $timeout + 1)
    fi
    if [ $timeout -eq 5 ];then
        if [ -z "$CtrlPID" ];then
            :
        else
            wait $CtrlPID
            kill -9 $CtrlPID
        fi
        break
    fi
done
./pq_server&
