project(tests)
cmake_minimum_required(VERSION 2.8)


include_directories(
    ./
    ../C-API/include
)


list(APPEND SOURCES
    main.cpp
    testDisplay.cpp
    testLeds.cpp
    testButtons.cpp
    testSplash.cpp
    infiniteDisplayButtonsTest.cpp
)


find_library(DHCOM_HAL DHCOM_HAL ../DHCOM_HAL)
find_library(POCO_FOUNDATION PocoFoundation)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    ${DHCOM_HAL}
    ${POCO_FOUNDATION}
    ${C_API}
)


