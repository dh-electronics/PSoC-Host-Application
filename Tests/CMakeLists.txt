cmake_minimum_required (VERSION 3.13)
project(psoc-drc02-tests)


include_directories(
    ./
    ${psoc-drc02_INCLUDE_DIRS}
)


list(APPEND SOURCES
    main.cpp
    testDisplay.cpp
    testLeds.cpp
    testButtons.cpp
    testSplash.cpp
    infiniteDisplayButtonsTest.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    dhcom-hal
    psoc-drc02
)


include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME
    DESTINATION bin
    COMPONENT bin
)

