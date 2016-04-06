#! /bin/bash
# $1 - source dir ($$PWD)
# $2 - poco dir ($$POCO)
# $3 - output dir ($$OUT_PWD)
# $4 - arch: armhf | armel
# $5 - service manager: systemd | update-rc

TMP=/tmp/drc-01
svn export --force $1/DEBIAN_PACKAGE $TMP
mkdir -p $TMP/etc $TMP/usr/lib $TMP/root

cd $2/lib
cp libPocoFoundation.so.31 libPocoNet.so.31 $TMP/usr/lib
cd -

cd $3
cp C-API/libc-api.so.1 DHCOM_HAL/libDHCOM_HAL.so.1 $TMP/usr/lib
cd -

cd $TMP/DEBIAN
if [[ $4 != "armel" && $4 != "armhf" ]] ; then
	echo ERROR: unknown cpu arch $4, must be armhf or armel
	exit -1
fi

if [[ $5 == "systemd" ]] ; then
	mv preinst.systemd preinst
	mv postinst.systemd postinst
	rm *.update-rc
else
	mv preinst.update-rc preinst
	mv postinst.update-rc postinst
	rm *.systemd
fi
cd -

dpkg -b $TMP $3/drc-01.deb
rm -rf $TMP
