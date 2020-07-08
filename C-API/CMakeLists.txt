cmake_minimum_required (VERSION 3.13)
project(psoc-drc02 VERSION 9.7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-std=c++11)
add_compile_options(-fPIC)

# hide  symbols of libc-api.so; visible symbols are set to default in includes
add_compile_options(-fvisibility=hidden)

set(C_API ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.so PARENT_SCOPE)

list(APPEND INCLUDES
    api/include/c-api/bitmap.h
    api/include/c-api/buttons.h
    api/include/c-api/c-api.h
    api/include/c-api/display.h
    api/include/c-api/helpers.h
    api/include/c-api/init.h
    api/include/c-api/leds.h
    api/include/c-api/resultcodes.h
    api/include/c-api/stats.h
    IApiManager.h
    ApiManager.h
    Compressor.h
    spi/Command.h
    spi/GenericHelpers.h
    spi/MasterHelpers.h
    spi/Response.h
    spi/SlaveHelpers.h
    spi/SpiProto.h
    peripherals/IButtons.h
    peripherals/IDisplay.h
    peripherals/ILed.h
    peripherals/Buttons.h
    peripherals/Display.h
    peripherals/Gpios.h
    peripherals/Led.h
    peripherals/LedOnGpio.h
)


list(APPEND SOURCES
    ApiManager.cpp
    helpers.cpp
    Compressor.cpp
    api/bitmap.cpp
    api/buttons.cpp
    api/display.cpp
    api/init.cpp
    api/leds.cpp
    api/stats.cpp
    spi/SpiProto.cpp
    spi/GenericHelpers.c
    spi/MasterHelpers.cpp
    peripherals/Buttons.cpp
    peripherals/Display.cpp
    peripherals/Led.cpp
    peripherals/LedOnGpio.cpp
)


file(GLOB public_headers "api/include/*.h")

add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${DHCOM_HAL_INCLUDE_DIRS}
)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        dhcom-hal
        "${CMAKE_THREAD_LIBS_INIT}"
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1)

# Expose C-API's public includes to other subprojects through cache variable
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                  ${PROJECT_SOURCE_DIR}/api/include
                                  ${DHCOM_HAL_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} 
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
    COMPONENT lib
)

# use this to install header files and keep directory structure
install(DIRECTORY api/include/ 
	DESTINATION include
	COMPONENT dev
	PATTERN "*.h"
)


