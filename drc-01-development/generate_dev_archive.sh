#! /bin/bash
# $1 - OUT_PWD

if [ $# != 1 ]; then
	echo OUT_PWD not supplied
	exit 1
fi

PACKAGE=drc01-dev
OUT_DIR=/var/tmp/$PACKAGE
rm -rf $OUT_DIR
mkdir -p $OUT_DIR/lib $OUT_DIR/bin $OUT_DIR/src 

svn export C-API/include $OUT_DIR/include && \
svn export Tests $OUT_DIR/src/Tests && \
svn export Examples $OUT_DIR/src/Examples && \
cp -P $1/C-API/libc-api.so* $1/DHCOM_HAL/libDHCOM_HAL.so* $OUT_DIR/lib && \
cp -P $1/Tests/tests $OUT_DIR/bin && \
cp -P $1/Examples/example $OUT_DIR/bin 

PLACE=$PWD
cd /var/tmp
tar cf $PLACE/$PACKAGE.tar.gz $PACKAGE 
cd -

rm -rf $OUT_DIR

