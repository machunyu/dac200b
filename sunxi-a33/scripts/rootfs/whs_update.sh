#!/bin/bash
#########################################
#Function:    update whs doorlock app & lib
#Usage:       bash whs_update.sh
#Author:      machunyu
#Email:       machunyu19891215@163.com
#Company:     Hongshi Company Ltd.
#Version:     1.0
#########################################

SERVER_IP="192.168.10.100"
APP_NAME=whs_arm_dlocker_display
USBLIB_NAME=libwhsusbapi.so
USBLIB_INI_NAME=/root/whsusbapi.ini
NETLIB_NAME=libarmclientapi.so
DEVICEINI=/home/share/resource/device.ini
WORK_HOME=/mnt
upFlag=0

getServerIP()                                                            
{
num=`grep "ServerIP" $DEVICEINI -n | awk -F: '{print $1}'`                
#echo $num
let num=num+1
name=`cat $DEVICEINI | sed -n "${num}p" | awk -F '=' '{print $1}'`
SERVER_IP=`cat $DEVICEINI | sed -n "${num}p" | awk -F '=' '{print $2}'`
#echo $name
echo "Server IP: $SERVER_IP"                                           
}

checkEnv()
{
for ((a = 1;a <=3; a++))
	do 
	if [ ! -d "/mnt/rollback" ];then
		mkdir -p /mnt/rollback
		cd /mnt/rollback
		mkdir -p app
		mkdir -p lib
		continue
	else
		echo "rollback already exists"
		break
	fi
done


if [ ! -d "/mnt/rollback" ];then
	echo "The rollback folder cannot be created"
	echo "NOW exit from the upgrade script"
	exit
else
	echo "create workspace folder successful"
fi

if [ "$APP_NAME" = "" ];then
	echo "Not comfirm the App name!"
	exit
fi
}

bak_app()
{
time=$(date '+%Y%m%d-%H%M%S' )
cd /mnt/rollback/app
if [ ! -f "$APP_NAME" ]; then
	mkdir $time
	cp /mnt/$APP_NAME ./$time
	sync
	echo "the WHS App has been completed backup"
fi
}

download_usb_lib()
{
cd /mnt/rollback/lib
tftp -l $USBLIB_NAME -g $SERVER_IP
if [ $? -eq 0 ];then
	sync
	NEW_FILE_LENGTH=$(ls -l ${USBLIB_NAME} | awk '{print $5}')
	cd /usr/lib
	OLD_FILE_LENGTH=$(ls -l ${USBLIB_NAME} | awk '{print $5}')
	if [ $NEW_FILE_LENGTH -ne $OLD_FILE_LENGTH ];then
		echo "downloading usb library ok!"
		cp /mnt/rollback/lib/$USBLIB_NAME /usr/lib -rdf
		let upFlag+=1;
		echo "upFlag: $upFlag"
	else
		echo "no need to update usb library"
	fi
else
	echo "tftp usb library error, maybe no need to update!"
fi
}

download_net_lib()
{
cd /mnt/rollback/lib
tftp -l $NETLIB_NAME -g $SERVER_IP
if [ $? -eq 0 ];then
	sync
	NEW_FILE_LENGTH=$(ls -l ${NETLIB_NAME} | awk '{print $5}')
	cd /usr/lib
	OLD_FILE_LENGTH=$(ls -l ${NETLIB_NAME} | awk '{print $5}')
	if [ $NEW_FILE_LENGTH -ne $OLD_FILE_LENGTH ];then
		echo "downloading network library ok!"
		cp /mnt/rollback/lib/$NETLIB_NAME /usr/lib -rdf
		let upFlag+=2;
		echo "upFlag: $upFlag"
	else
		echo "no need to update network library"
	fi
else
	echo "tftp net library error, maybe no need to update!"
fi
}

download_app()
{
cd /mnt/rollback/app
tftp -l $APP_NAME -g $SERVER_IP
if [ $? -eq 0 ];then
	sync
	echo "Now pack is downloading ready ..."
	NEW_FILE_LENGTH=$(ls -l ${APP_NAME} | awk '{print $5}')
	cd /mnt
	OLD_FILE_LENGTH=$(ls -l ${APP_NAME} | awk '{print $5}')
	if [ $NEW_FILE_LENGTH -ne $OLD_FILE_LENGTH ];then
		echo "downloading whs app ok, now will start to stop app and update"
		let upFlag+=4;
		echo "upFlag: $upFlag"
	else
		echo "no need to update applation image"
	fi
else
	echo "tftp whs applation error, maybe no need to update!"
fi
}

stop_app()
{
#for ((i = 1; i <= 3 ; i++)); do 
	########################################################
	num=`ps -ef |grep "$APP_NAME"|grep -v "grep"|awk '{print $1}'`
	########################################################
	echo "process id:$num"
	killall -9 $APP_NAME
	#if [ ! -n "$num" ];then
	#	kill -9 $num
	#else
	#	echo "The $APP_NAME process has been killed" 
	#	break
	#fi
#done
}

update_app()
{
echo "rm usb config file"
rm -rf $USBLIB_INI_NAME
cd /mnt/rollback/app
FILE_LENGTH=$(ls -l ${APP_NAME} | awk '{print $5}')
if [ $FILE_LENGTH -gt 0 ];then
	cp $APP_NAME /mnt/$APP_NAME
	sync
	echo "The app update is ok ..."
else
	echo "[${APP_NAME}] is bad or broken."
	echo "remove the Null-file [${APP_NAME}]"
	rm -rf $APP_NAME
fi
}

restart_app()
{
cd /mnt
chmod +x $APP_NAME
echo "----------------------------------\n"
echo "run: {${APP_NAME}}"
/bin/bash /mnt/run_qt.sh
echo "----------------------------------\n"
}

checkEnv
getServerIP
download_app
download_usb_lib
download_net_lib
if [ $upFlag -ne 0 ]; then
#	if [ $upFlag -ge 4 ]; then
#		bak_app
#	else
#		echo "only update lib, no need to back app!"
#	fi
	stop_app
	update_app
	restart_app
fi
