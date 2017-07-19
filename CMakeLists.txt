cmake_minimum_required(VERSION 3.1)
project(logger)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wbad-function-cast")
endif ()

#####
# Archive
###
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
