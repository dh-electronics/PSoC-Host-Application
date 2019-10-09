project(c-api)
cmake_minimum_required(VERSION 2.8)


set(C_API ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.so PARENT_SCOPE)


include_directories(
    ./
    api/include
    ${DHCOM_HAL_INCLUDE_DIRS}
)


list(APPEND SOURCES
    api/include/c-api/resultcodes.h
    api/include/c-api/c-api.h
    api/include/c-api/init.h
    api/include/c-api/stats.h
    api/include/c-api/display.h
    api/include/c-api/buttons.h
    api/include/c-api/leds.h
    api/include/c-api/helpers.h
    api/include/c-api/bitmap.h
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
)

list(APPEND SOURCES
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
find_package(Threads REQUIRED)

file(GLOB public_headers "api/include/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${INCLUDES} )
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
target_link_libraries(${PROJECT_NAME}
    DHCOM_HAL
    "${CMAKE_THREAD_LIBS_INIT}"
)

# Expose C-API's public includes to other subprojects through cache variable
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                  ${PROJECT_SOURCE_DIR}/api/include
                                  ${DHCOM_HAL_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

install(TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/c-api
)


