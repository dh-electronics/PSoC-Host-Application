TEMPLATE = subdirs
CONFIG -= QT

C-API.file = C-API/C-API.pro
JSON-API.file = JSON-API/JSON-API.pro
DHCOM_HAL.file = DHCOM_HAL/DHCOM_HAL.pro
SUBDIRS = DHCOM_HAL C-API #JSON-API

C-API.depends = DHCOM_HAL
JSON-API.depends = C-API

OTHER_FILES=generate_debian_package.sh

CONFIG(DHCOM_iMX6_rev300){
    ARCH=armhf
    INIT=systemd
} else {
    ARCH=armel
    INIT=init.d
}

debian.path = $$OUT_PWD
debian.commands = $$PWD/generate_debian_package.sh $$PWD $$debian.path $$ARCH $$INIT
INSTALLS = debian
