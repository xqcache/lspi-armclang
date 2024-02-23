

set(PRIVATE_SOURCES
    bsp_led.h
    bsp_led.cpp
    bsp_serial.h
    bsp_serial.cpp
    bsp_i2c.h
    bsp_i2c.cpp
    bsp_mpu6050.h
    bsp_mpu6050.cpp
)

target_sources(${TARGET_NAME} PRIVATE ${PRIVATE_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
