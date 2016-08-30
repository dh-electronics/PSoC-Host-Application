TARGET = c-api
TEMPLATE = lib
CONFIG -= qt
CONFIG += shared #DUMMY_PERIPHERALS


message("Poco is in $$POCO")

HEADERS = \
include/resultcodes.h \
include/c-api.h \
include/init.h \
include/stats.h \
include/display.h \
include/buttons.h \
include/leds.h \
include/eeprom.h \
include/helpers.h \
ApiManager.h \
spi/Command.h \
spi/Response.h \
spi/GenericHelpers.h \
spi/MasterHelpers.h \
spi/SpiProto.h \
peripherals/ILed.h \
peripherals/IButtons.h \
peripherals/IDisplay.h \
peripherals/IEeprom.h \
peripherals/IApiManager.h \
peripherals/Led.h \
peripherals/LedOnGpio.h \
peripherals/Buttons.h \
peripherals/Display.h \
peripherals/Gpios.h \
peripherals/Eeprom.h \
    Compressor.h \
    include/bitmap.h

SOURCES = \ 
api/buttons.cpp \
api/display.cpp \
api/leds.cpp \
api/stats.cpp \
api/init.cpp \
api/eeprom.cpp \
ApiManager.cpp \
spi/GenericHelpers.c \
spi/MasterHelpers.cpp \
peripherals/Led.cpp \
helpers.cpp \
    Compressor.cpp \
    api/bitmap.cpp

CONFIG(DHCOM_AM33) || CONFIG(DHCOM_iMX6_rev300) {
    message("Will compile REAL peripherals.")

    SOURCES += \
        spi/SpiProto.cpp \
        peripherals/LedOnGpio.cpp \
        peripherals/Buttons.cpp \
        peripherals/Display.cpp \
        peripherals/Eeprom.cpp

    LIBS += -L../DHCOM_HAL -lDHCOM_HAL

} else {
    error("Must be compiled for DHCOM_AM35 or DHCOM_iMX6_rev300 architecture!")
}

INCLUDEPATH += include /usr/include/Poco ../DHCOM_HAL/include

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
