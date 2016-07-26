#! /bin/bash
# $1 - OUT_PWD

if [ $# != 1 ]; then
	echo OUT_PWD not supplied
	exit 1
fi

OUT_DIR=/var/tmp/drc01-dev
rm -rf $OUT_DIR
mkdir -p $OUT_DIR/lib $OUT_DIR/bin 

svn export C-API/include $OUT_DIR/include && \
svn export Tests $OUT_DIR/src && \
cp -P $1/C-API/libc-api.so* $1/DHCOM_HAL/libDHCOM_HAL.so* $OUT_DIR/lib && \
cp -P $1/Tests/tests $OUT_DIR/bin && \
tar cf drc_01-dev.tar.gz $OUT_DIR

rm -rf $OUT_DIR

