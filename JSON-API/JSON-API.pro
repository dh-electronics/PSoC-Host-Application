TARGET = json-server
TEMPLATE = app
CONFIG -= qt

message("POCO is supposed to be at: $$POCO, make and install it by: $cd POCO ; configure-dhcom.sh ; sudo make install")

HEADERS = \
    ServiceListener.h \
    JsonServer.h \
    JsonState.h \
    WriteStream.h \
    ReadStream.h

SOURCES = \ 
    main.cpp \
    ServiceListener.cpp \
    JsonServer.cpp \
    JsonHandlers.cpp \
    JsonState.cpp \
    WriteStream.cpp \
    ReadStream.cpp

INCLUDEPATH = $$POCO/include rapidjson-master/include ../C-API/include
message("Includes: " $$INCLUDEPATH)

LIBS += -L$$POCO/lib -L../C-API -lc-api -L../DHCOM_HAL -lDHCOM_HAL -lpthread

CONFIG(debug, debug | release) {
    message("Configuring DEBUG build")
    LIBS += -lPocoFoundationd -lPocoNetd
    QMAKE_CXXFLAGS += -g3 -O0

    target.path = /root

    poco.path = /root
    poco.files = $$POCO/lib/libPocoFoundationd.so.16 $$POCO/lib/libPocoNetd.so.16

    api.path = $$poco.path
    api.files = $$OUT_PWD/../C-API/libc-api.so.1 $$OUT_PWD/../C-API/libc-api.so.1.0.0

    hal.path = $$poco.path
    hal.files = $$OUT_PWD/../DHCOM_HAL/libDHCOM_HAL.so.1 $$OUT_PWD/../DHCOM_HAL/libDHCOM_HAL.so.1.0.0

    INSTALLS = target hal api poco
}

CONFIG(release, debug | release) {
    message("Configuring RELEASE build")
    LIBS += -lPocoFoundation -lPocoNet
    QMAKE_CXXFLAGS += -g0 -O3

    debian_package_path = /tmp/drc-01
    target.path = $$debian_package_path/root

    poco.path = $$debian_package_path/usr/lib
    poco.files = $$POCO/lib/libPocoFoundation.so.16 $$POCO/lib/libPocoNet.so.16

    api.path = $$poco.path
    api.files = $$OUT_PWD/../C-API/libc-api.so.1

    hal.path = $$poco.path
    hal.files = $$OUT_PWD/../DHCOM_HAL/libDHCOM_HAL.so.1

    debian.path = $$debian_package_path
    debian.commands = \
svn export --force $$PWD/../DEBIAN_PACKAGE $$debian_package_path ;\
dpkg -b $$debian_package_path $$OUT_PWD/../drc-01.deb ;\
rm -rf $$debian_package_path

    INSTALLS = target hal api poco debian
}


message("Libs: " $$LIBS)

DEFINES += __STDC_CONSTANT_MACROS