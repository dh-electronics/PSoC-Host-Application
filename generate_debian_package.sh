#! /bin/bash
# $1 - source dir ($$PWD)
# $2 - output dir ($$OUT_PWD)
# $3 - arch: armhf | armel
# $4 - init manager: systemd | init.d

if [[ $3 != "armel" && $3 != "armhf" ]] ; then
	echo ERROR: unknown cpu arch $3, must be armhf|armel
	exit -1
fi

TMP=/tmp/drc-01
svn export --force $1/DEBIAN_PACKAGE $TMP
mkdir -p $TMP/usr/lib $TMP/usr/bin

cd $2
cp C-API/libc-api.so.1 $TMP/usr/lib
cp Tool/ui-tool $TMP/usr/bin
cd -

cd $TMP/DEBIAN
sed "s/Architecture:.*/Architecture:$3/" <control >control.edited
mv control.edited control
cd -

dpkg -b $TMP $2/drc-01.deb
rm -rf $TMP
