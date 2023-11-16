cmake_minimum_required(VERSION 3.22.1)
project(mongoose C)

set(SOURCES mongoose.c)

add_library(${PROJECT_NAME} ${SOURCES})

if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} m)
endif ()


