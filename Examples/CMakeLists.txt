project(example)
cmake_minimum_required(VERSION 2.8)


include_directories(
    ./
    ../C-API/include
)


list(APPEND SOURCES
    bounce.h
    space.h
    main.cpp
    bounce.cpp
    space.cpp
)


find_library(POCO_FOUNDATION PocoFoundation)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(
    ${PROJECT_NAME}
    ${DHCOM_HAL}
    ${POCO_FOUNDATION}
    ${C_API}
)

