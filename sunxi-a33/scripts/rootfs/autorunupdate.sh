#!/bin/sh
#该脚本的功能为每个工作日的11点32分执行一次ps命令
#Section configuration
#Task Time ,example:203000(Time 20:30:00);190000(Time 19:00:00);  
startTime=040100
#the programs you want to execute
program=updateApp

#Section promgram
perDate=$(date "+%Y%m%d")
isNewDay=1  
isFirstTime=1

echo 'Task schedule Time: ('$startTime') program: ('$program') Waiting...'  
  
while true ; do  
	curTime=$(date "+%H%M%S")
	curDate=$(date "+%Y%m%d")

	#Check week day
	week=`date +%w`
	if [ $week -eq 6 ] || [ $week -eq 0 ];then
		isNewDay=0
		sleep 1
		continue

	else
		#check and run script
		if [ "$isNewDay" -eq "1" ];then
			if [ "$curTime" -gt "$startTime" ];then
				if [ "$isFirstTime" -eq "0" ];then
					echo 'The program ('$program') Running...'
					#$program
					/bin/bash /etc/scripts/whs_update.sh
					echo 'The program ('$program') Stopped...'
				fi
				isNewDay=0
			else
				if [ "$isFirstTime" -eq "1" ];then
					echo 'New Day: ('$curDate') Task schedule Time: ('$startTime') Waiting...'
					isFirstTime=0
				fi
			fi
		else
			#new day start
			if [ "$curDate" -gt "$perDate" ];then
				echo 'New Day: ('$curDate') Task schedule Time: ('$startTime') Waiting...'
				isNewDay=1
				perDate=$curDate
			fi
		fi
		sleep 1
	fi
done
