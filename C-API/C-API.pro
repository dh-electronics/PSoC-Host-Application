TARGET = c-api
TEMPLATE = lib
CONFIG -= qt
CONFIG += shared #DUMMY_PERIPHERALS

message("POCO is supposed to be at: $$POCO, make and install it by: $cd POCO ; configure-dhcom.sh ; sudo make install")

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
    Compressor.h

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
    Compressor.cpp

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


INCLUDEPATH += include $$POCO/include ../DHCOM_HAL/include

LIBS += -L$$POCO/lib

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
