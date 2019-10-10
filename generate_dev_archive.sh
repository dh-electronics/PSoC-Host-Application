#! /bin/bash
# $1 - SRC_DIR
# $2 - BUILD_DIR

if [ $# -ne 2 ]; then
	echo two args must be supplied - SRC_DIR BUILD_DIR
	exit 1
fi

PACKAGE=psoc-capi-drc02-dev
OUT_DIR=/var/tmp/$PACKAGE
rm -rf $OUT_DIR
mkdir -p $OUT_DIR/lib $OUT_DIR/bin $OUT_DIR/src 

cp -R $1/C-API/api/include $OUT_DIR/include && \
cp -R $1/Tests $OUT_DIR/src/Tests && \
cp -R $1/Examples $OUT_DIR/src/Examples && \
cp -P $2/C-API/libc-api.so* $2/DHCOM_HAL/libDHCOM_HAL.so* $OUT_DIR/lib && \
cp -P $2/Tests/tests $OUT_DIR/bin && \
cp -P $2/Examples/example $OUT_DIR/bin 

PLACE=$PWD
cd /var/tmp
tar cf $PLACE/$PACKAGE.tar.gz $PACKAGE 
cd -

rm -rf $OUT_DIR

