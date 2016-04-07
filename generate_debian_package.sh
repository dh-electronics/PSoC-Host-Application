#! /bin/bash
# $1 - source dir ($$PWD)
# $2 - output dir ($$OUT_PWD)
# $3 - arch: armhf | armel
# $4 - init manager: systemd | init.d

if [[ $3 != "armel" && $3 != "armhf" ]] ; then
	echo ERROR: unknown cpu arch $3, must be armhf|armel
	exit -1
fi

if [[ $4 != "systemd" && $4 != "init.d" ]] ; then
	echo ERROR: unknown init manager $4, must be systemd|init.d 
	exit -1
fi

TMP=/tmp/drc-01
svn export --force $1/DEBIAN_PACKAGE $TMP
mkdir -p $TMP/usr/lib $TMP/root

cd $2
cp C-API/libc-api.so.1 DHCOM_HAL/libDHCOM_HAL.so.1 $TMP/usr/lib
cp JSON-API/json-server $TMP/root
cd -

cd $TMP/DEBIAN
sed "s/Architecture:.*/Architecture:$3/" <control >control.edited
mv control.edited control

if [[ $4 == "systemd" ]] ; then
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

dpkg -b $TMP $2/drc-01.deb
rm -rf $TMP
