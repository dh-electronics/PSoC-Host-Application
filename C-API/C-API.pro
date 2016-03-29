TARGET = c-api
TEMPLATE = lib
CONFIG -= qt
CONFIG += shared #DUMMY_PERIPHERALS

POCO = /opt/poco-1.4.6p4-arm
message("POCO is supposed to be at: $$POCO, make and install it by: $cd POCO ; configure-dhcom.sh ; sudo make install")

HEADERS = \
include/resultcodes.h \
include/c-api.h \
include/init.h \
include/stats.h \
include/inputs.h \
include/relais.h \
include/dacs.h \
include/display.h \
include/buttons.h \
include/leds.h \
include/eeprom.h \
include/IInput.h \
include/IRelay.h \
include/IDac.h \
include/ILed.h \
include/IButtons.h \
include/IDisplay.h \
include/IEeprom.h \
include/IApiManager.h \
include/helpers.h \
ApiManager.h \
spi/Command.h \
spi/Response.h \
spi/GenericHelpers.h \
spi/MasterHelpers.h \
spi/SpiProto.h \
peripherals/Input.h \
peripherals/Relay.h \
peripherals/Dac.h \
peripherals/Led.h \
peripherals/LedOnGpio.h \
peripherals/LedOnSR.h \
peripherals/Buttons.h \
peripherals/Display.h \
peripherals/Gpios.h \
peripherals/Eeprom.h \
    spi/InputMode.h \
    peripherals/SR.h

SOURCES = \ 
api/buttons.cpp \
api/display.cpp \
api/inputs.cpp \
api/relais.cpp \
api/dacs.cpp \
api/leds.cpp \
api/stats.cpp \
api/init.cpp \
api/eeprom.cpp \
ApiManager.cpp \
spi/GenericHelpers.c \
spi/MasterHelpers.cpp \
peripherals/Led.cpp \
helpers.cpp \
    peripherals/SR.cpp

CONFIG(DUMMY_PERIPHERALS) {
    message("Will compile DUMMY peripherals.")
    DEFINES += DUMMY_PERIPHERALS

    SOURCES += \
    spi/SpiProtoDummy.cpp \
    peripherals/InputDummy.cpp \
    peripherals/RelayDummy.cpp \
    peripherals/DacDummy.cpp \
    peripherals/LedOnGpioDummy.cpp \
    peripherals/LedOnSRDummy.cpp \
    peripherals/ButtonsDummy.cpp \
    peripherals/DisplayDummy.cpp

} else {
    CONFIG(DHCOM_AM35) {
        message("Will compile REAL peripherals.")

        SOURCES += \
        spi/SpiProto.cpp \
        peripherals/Input.cpp \
        peripherals/Input1v4.cpp \
        peripherals/Input2v0.cpp \
        peripherals/Relay.cpp \
        peripherals/Dac.cpp \
        peripherals/LedOnGpio.cpp \
        peripherals/LedOnSR.cpp \
        peripherals/Buttons.cpp \
        peripherals/Display.cpp \
        peripherals/Eeprom.cpp

        LIBS += -L../DHCOM_HAL -lDHCOM_HAL

    } else {
        error("Must be compiled for DHCOM_AM35 architecture!")
    }
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
