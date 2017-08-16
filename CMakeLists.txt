cmake_minimum_required(VERSION 3.1)
project(logger)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wbad-function-cast")
endif ()

#####
# Woring direcotries
###
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/deps)

#####
# Dependencies
###
include(${PROJECT_SOURCE_DIR}/deps/sds/sds.cmake)

#####
# Archive
###
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE sds)

#####
# Tests
###
file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/test_*.c)
foreach (source_file ${TEST_SOURCES})
    get_filename_component(target ${source_file} NAME_WE)
    add_executable(${target} ${source_file})
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
    add_test(${target} ${target})
endforeach (source_file ${TEST_SOURCES})

#####
# Examples
###
file(GLOB EXAMPLE_SOURCES ${PROJECT_SOURCE_DIR}/examples/example_*.c)
foreach (source_file ${EXAMPLE_SOURCES})
    get_filename_component(target ${source_file} NAME_WE)
    add_executable(${target} ${source_file})
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
endforeach (source_file ${EXAMPLE_SOURCES})
