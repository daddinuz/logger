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
add_executable(test_logger_message test/test_logger_message.c)
target_link_libraries(test_logger_message PRIVATE ${PROJECT_NAME})

add_executable(test_logger_formatter test/test_logger_formatter.c)
target_link_libraries(test_logger_formatter PRIVATE ${PROJECT_NAME})

#####
# Examples
###
add_executable(example examples/example.c)
target_link_libraries(example PRIVATE ${PROJECT_NAME})
