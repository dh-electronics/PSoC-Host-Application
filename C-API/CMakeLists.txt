project(c-api)
cmake_minimum_required(VERSION 2.8)


set(C_API ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.so PARENT_SCOPE)


include_directories(
    ./
    include
    /usr/include
    ${DHCOM_HAL_INCLUDE_DIRS}
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
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1)
target_link_libraries(${PROJECT_NAME}
    DHCOM_HAL
    ${POCO_FOUNDATION}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# Expose DHCOM_HAL's public includes to other subprojects through cache variable
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                  ${PROJECT_SOURCE_DIR}/include
                                  ${PROJECT_SOURCE_DIR}/usr/include
                                  ${DHCOM_HAL_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION /lib)

