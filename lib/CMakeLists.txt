file(GLOB_RECURSE PRIVATE_SOURCES
    "*.c" "*.cpp" "*.h" "*.s"
)

target_sources(${TARGET_NAME} PRIVATE ${PRIVATE_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_options(${TARGET_NAME} PRIVATE --scatter=${CMAKE_CURRENT_SOURCE_DIR}/gd32f4xx.sct)