#!/bin/bash
FILE=showPics

if [ ! $1 ]; then
  echo "Compile as x86"
  TARGET=$FILE"_"$1
else
  echo "Compile as arm"
  TARGET=$FILE"_"$1
  echo $TARGET
fi

export PATH=$PATH:/home/embest/Documents/A33/Linux/dragonboard/out/sun8iw5p1/dragonboard/common/buildroot/external-toolchain/bin
export PATH=$PATH:/opt/hs_project/sunxi/hslib/qtlib5.6.2-arm/bin/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/hs_project/sunxi/hslib/qtlib5.6.2-arm/lib/

make distclean
qmake-arm-5.6.2 -project -o $TARGET.pro
sed -i '4a QT       += core gui' $TARGET.pro
sed -i '5a greaterThan(QT_MAJOR_VERSION, 4): QT += widgets' $TARGET.pro
sed -i '6a \n' $TARGET.pro
sed -i '7a MOC_DIR=arm/moc' $TARGET.pro
sed -i '8a RCC_DIR=arm/rcc' $TARGET.pro
sed -i '9a UI_DIR=arm/ui' $TARGET.pro
sed -i '10a OBJECTS_DIR=arm/obj' $TARGET.pro
sed -i '11a \n' $TARGET.pro

qmake-arm-5.6.2 $TARGET.pro

make -j8

cp $TARGET /home/embest/Documents/tftboot

echo "copy $TARGET image ok"
