#! /bin/bash
# $1 - source dir ($$PWD)
# $2 - output/build dir ($$OUT_PWD)
# $3 - arch: armhf | armel
# $4 - init manager: systemd | init.d

if [[ $3 != "armel" && $3 != "armhf" ]] ; then
	echo ERROR: unknown cpu arch $3, must be armhf|armel
	exit -1
fi

TMP=/tmp/drc-01
mkdir -p $TMP/

#svn export --force $1/DEBIAN_PACKAGE $TMP
cp -R $1/DEBIAN_PACKAGE/* $TMP
mkdir -p $TMP/usr/lib $TMP/usr/bin

# get version.revision from source code (stats.h)
cd $1/C-API/
VERSION=$(grep -r "^#define API_VERSION" . | sed "s/[^0-9]*//g")
REVISION=$(grep -r "^#define API_REVISION" . | sed "s/[^0-9]*//g")

cd $2
cp C-API/libc-api.so.1 $TMP/usr/lib
cp Tool/ui-tool $TMP/usr/bin
cp Bootloader-Host/bootloader-tool $TMP/usr/bin
cd -

cd $TMP/DEBIAN
sed -i "s/Architecture:.*/Architecture:$3/" control
sed -i "s/Version:.*/Version:$VERSION\.$REVISION/" control
cd -

dpkg -b $TMP $2/drc-01_"$VERSION"."$REVISION"_"$3".deb
rm -rf $TMP
