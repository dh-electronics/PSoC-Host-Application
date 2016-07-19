TARGET = tests
TEMPLATE = app
CONFIG -= qt

HEADERS = 

SOURCES = \  
    main.cpp

INCLUDEPATH += $$PWD/../C-API/include

LIBS += -L$$POCO/lib -L$$OUT_PWD/../C-API/ -lc-api -L../DHCOM_HAL -lDHCOM_HAL

CONFIG(debug, debug | release) {
    message("Configuring DEBUG build")
    LIBS += -lPocoFoundationd
    QMAKE_CXXFLAGS += -g3 -O0
}

CONFIG(release, debug | release) {
    message("Configuring RELEASE build")
    LIBS += -lPocoFoundation
    QMAKE_CXXFLAGS += -g0 -O3
}

DEPENDPATH += $$PWD/../C-API

#target.path = /root
#INSTALLS = target
