TARGET = tests
TEMPLATE = app
CONFIG -= qt

SOURCES = \
    main.cpp \
    testDisplay.cpp \
    testLeds.cpp \
    testButtons.cpp \
    testSplash.cpp

INCLUDEPATH += \
    $$PWD/../C-API/include

LIBS += \
    -L../C-API -lc-api \
    -L../DHCOM_HAL -lDHCOM_HAL \
    -L$$POCO/lib


DEPENDPATH += $$PWD/../C-API


CONFIG(debug, debug | release) {
    message("Configuring DEBUG build")
    LIBS +=  -lPocoFoundationd
}


CONFIG(release, debug | release) {
    message("Configuring RELEASE build")
    LIBS += -lPocoFoundation
}
