project(ui-tool)
cmake_minimum_required(VERSION 2.8)


include_directories(
    ../C-API/include
    ${POCO}/include
)


list(APPEND SOURCES
    Service.h
    Display.h
    Leds.h
    Buttons.h
    Api.h
    main.cpp
    Service.cpp
    Display.cpp
    Leds.cpp
    Buttons.cpp
    Api.cpp
)


find_library(POCO_FOUNDATION PocoFoundation)
find_library(DHCOM_HAL DHCOM_HAL)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    ${DHCOM_HAL}
    ${POCO_FOUNDATION}
    c-api
)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

