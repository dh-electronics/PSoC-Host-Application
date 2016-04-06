#! /bin/bash
# $1 - source dir ($$PWD)
# $2 - poco dir ($$POCO)
# $3 - output dir ($$OUT_PWD)
# $4 - arch: armhf | armel
# $5 - service manager: systemd | init.d

if [[ $4 != "armel" && $4 != "armhf" ]] ; then
	echo ERROR: unknown cpu arch $4, must be armhf|armel
	exit -1
fi

if [[ $5 != "systemd" && $5 != "init.d" ]] ; then
	echo ERROR: unknown service manager $5, must be systemd|init.d 
	exit -1
fi

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
sed "s/Architecture:.*/Architecture:$4/" <control >control.edited
mv control.edited control

if [[ $5 == "systemd" ]] ; then
	mv preinst.systemd preinst
	mv postinst.systemd postinst
	rm *.update-rc
	cd - ; cd $TMP/etc
	rm -rf init.d
else
	mv preinst.update-rc preinst
	mv postinst.update-rc postinst
	rm *.systemd
	cd - ; cd $TMP/etc
	rm -rf systemd 
fi
cd -

dpkg -b $TMP $3/drc-01.deb
rm -rf $TMP
