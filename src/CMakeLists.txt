set(PRIVATE_SOURCES
    main.cpp
    gd32f4xx_it.c
    gd32f4xx_it.h
    systick.c
    systick.h
)

target_sources(${TARGET_NAME} PRIVATE ${PRIVATE_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(bsp)