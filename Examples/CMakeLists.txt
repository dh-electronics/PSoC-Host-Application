project(example)
cmake_minimum_required(VERSION 2.8)


include_directories(
    ./
    ${c-api_INCLUDE_DIRS}
)


list(APPEND SOURCES
    bounce.h
    space.h
    main.cpp
    bounce.cpp
    space.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(
    ${PROJECT_NAME}
    DHCOM_HAL
    c-api
)

