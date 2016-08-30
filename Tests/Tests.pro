TARGET = tests
TEMPLATE = app
CONFIG -= qt

HEADERS = \ 
    bounce.h \
    space.h

SOURCES = \  
    main.cpp \
    bounce.cpp \
    space.cpp

INCLUDEPATH += $$PWD/../C-API/include

LIBS += -L$$OUT_PWD/../C-API/ -lc-api -L../DHCOM_HAL -lDHCOM_HAL

CONFIG(debug, debug | release) {
    message("Configuring DEBUG build")
    LIBS += $$POCO/lib/libPocoFoundationd.so.9
    QMAKE_CXXFLAGS += -g3 -O0
}

CONFIG(release, debug | release) {
    message("Configuring RELEASE build")
    LIBS += $$POCO/lib/libPocoFoundation.so.9
    QMAKE_CXXFLAGS += -g0 -O3
}

DEPENDPATH += $$PWD/../C-API

#target.path = /root
#INSTALLS = target
