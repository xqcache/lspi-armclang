cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)

string(REGEX REPLACE "[/\\]$" "" PROJECT_PATH ${CMAKE_SOURCE_DIR})
string(REGEX REPLACE ".*/(.*)$" "\\1" TARGET_NAME ${PROJECT_PATH})

project(${TARGET_NAME} VERSION 0.0.1 LANGUAGES C ASM CXX)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

add_executable(${TARGET_NAME})

target_compile_definitions(${TARGET_NAME} PRIVATE 
    $<$<COMPILE_LANGUAGE:CXX>:${COMPILER_DEFINES}>
    $<$<COMPILE_LANGUAGE:C>:${COMPILER_DEFINES}>
    -D__MICROLIB

)
target_link_options(${TARGET_NAME} PRIVATE
    --library_type=microlib
    --list=${CMAKE_BINARY_DIR}/${TARGET_NAME}.map
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${TARGET_NAME} PRIVATE -O0)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -O2)
endif()

add_subdirectory(lib)
add_subdirectory(src)


add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND fromelf $<TARGET_FILE:${TARGET_NAME}> --i32 --output=$<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.hex
    COMMAND fromelf $<TARGET_FILE:${TARGET_NAME}> --bin --output=$<TARGET_FILE_DIR:${TARGET_NAME}>/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.bin
)