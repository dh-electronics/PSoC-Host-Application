cmake_minimum_required (VERSION 3.13)
project(psoc-ui-tool)


include_directories(
    ${psoc-drc02_INCLUDE_DIRS}
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
    dhcom-hal
    psoc-drc02
)


target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)


include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
    COMPONENT bin
)

