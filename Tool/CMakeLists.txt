project(psoc-ui-tool)
cmake_minimum_required(VERSION 2.8)


include_directories(
    ${c-api_INCLUDE_DIRS}
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


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    DHCOM_HAL
    c-api
)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

