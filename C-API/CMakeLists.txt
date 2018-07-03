project(c-api)
cmake_minimum_required(VERSION 2.8)


include(CPack)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter Kishalov <pkishalov@dh-electronics.de>" CACHE STRING "Maintainer of the Debian Package")
set(CPACK_DEBIAN_PACKAGE_VERSION "0.1" CACHE STRING "Version of the Debian Package")


set(C_API ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.so PARENT_SCOPE)


include_directories(
    ./
    include
    /usr/include
    ../DHCOM_HAL/include
)


list(APPEND SOURCES
    include/resultcodes.h
    include/c-api.h
    include/init.h
    include/stats.h
    include/display.h
    include/buttons.h
    include/leds.h
    include/helpers.h
    include/bitmap.h
    IApiManager.h
    ApiManager.h
    Compressor.h
    spi/Command.h
    spi/Response.h
    spi/GenericHelpers.h
    spi/MasterHelpers.h
    spi/SpiProto.h
    peripherals/ILed.h
    peripherals/IButtons.h
    peripherals/IDisplay.h
    peripherals/Led.h
    peripherals/LedOnGpio.h
    peripherals/Buttons.h
    peripherals/Display.h
    peripherals/Gpios.h
    peripherals/Eeprom.h
    ApiManager.cpp
    helpers.cpp
    Compressor.cpp
    api/buttons.cpp
    api/display.cpp
    api/leds.cpp
    api/stats.cpp
    api/init.cpp
    api/bitmap.cpp
    spi/SpiProto.cpp
    spi/GenericHelpers.c
    spi/MasterHelpers.cpp
    peripherals/Led.cpp
    peripherals/LedOnGpio.cpp
    peripherals/Buttons.cpp
    peripherals/Display.cpp
)


find_library(POCO_FOUNDATION PocoFoundation)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    ${DHCOM_HAL}
    ${POCO_FOUNDATION}
)

install_targets(/lib ${PROJECT_NAME})

