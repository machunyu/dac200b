#!/bin/sh
while [ true ];do
	echo "Weclome to hongshi"
	echo -n "login: "
	read name
	echo -n "Password: "
	stty -echo
	read passwd
	stty echo
	echo ""
	if [ -z ${name} ]; then
		continue
	fi
	echo ${passwd} | md5sum -c -s /home/${name}/.passwd > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		break
	else
		continue
	fi
done
