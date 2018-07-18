#!/bin/sh
export PATH=$PATH:/home/embest/Documents/ext/arm-linux-gnueabihf-4.9/bin
echo "build libusb ..."
rm -rf libusb-1.0.20
unzip libusb-1.0.20.zip
chmod -R 777 libusb-1.0.20
cd libusb-1.0.20
make distclean
./configure --host=arm-linux-gnueabihf --prefix=/home/embest/Documents/ext/hslib --disable-udev
make && make install
echo "build libusb ok"

echo "build glog ..."
cd ..
rm -rf glog-0.3.4
unzip glog-0.3.4.zip
chmod -R 777 glog-0.3.4
cd glog-0.3.4
make distclean
./configure --host=arm-linux-gnueabihf --prefix=/home/embest/Documents/ext/hslib
make && make install
echo "build glog ok"

echo "build libwhsusbapi ..."
cd ../whs_device_usb_api_v4_3_9/api/whsusbapi/whsusbapi
make clean
make
cp libwhsusbapi.so /home/embest/Documents/ext/hslib/lib
echo "build libwhsusbapi ok"

echo "tar hslib"
cd /home/embest/Documents/ext
tar -cjvf hslib.tar.bz2 hslib
