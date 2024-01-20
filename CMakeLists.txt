# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(dispenser_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)

# Tell CMake where to find the executable source file
add_executable(dispenser_project
        main.c
        functions.c
        functions.h
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(dispenser_project)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(dispenser_project
        pico_stdlib
        hardware_pwm
        hardware_gpio
        hardware_i2c
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(dispenser_project 0)
pico_enable_stdio_uart(dispenser_project 1)
