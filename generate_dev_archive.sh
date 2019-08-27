#! /bin/bash
# $1 - OUT_PWD $2 - DHCOM_HAL

if [ $# -ne 2 ]; then
	echo two args must be supplied - OUT_PWD and DHCOM_HAL
	exit 1
fi

PACKAGE=drc02-dev
OUT_DIR=/var/tmp/$PACKAGE
rm -rf $OUT_DIR
mkdir -p $OUT_DIR/lib $OUT_DIR/bin $OUT_DIR/src 

cp -R $1/C-API/api/include $OUT_DIR/include && \
cp -R $1/Tests $OUT_DIR/src/Tests && \
cp -R $1/Examples $OUT_DIR/src/Examples && \
cp -P $1/C-API/libc-api.so* $2/libDHCOM_HAL.so* $OUT_DIR/lib && \
cp -P $1/Tests/tests $OUT_DIR/bin && \
cp -P $1/Examples/example $OUT_DIR/bin 

PLACE=$PWD
cd /var/tmp
tar cf $PLACE/$PACKAGE.tar.gz $PACKAGE 
cd -

rm -rf $OUT_DIR

