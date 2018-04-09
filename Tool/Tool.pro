TARGET = ui-tool
TEMPLATE = app
CONFIG -= qt


HEADERS = \
    Service.h \
    Display.h \
    Leds.h \
    Buttons.h \
    Api.h


SOURCES = \
    main.cpp \
    Service.cpp \
    Display.cpp \
    Leds.cpp \
    Buttons.cpp \
    Api.cpp


INCLUDEPATH += \
    $$PWD/../C-API/include \
    $$POCO/include


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
