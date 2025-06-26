# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Main Main.c )
add_subdirectory(lib/I2S)
add_subdirectory(lib/PWM)
add_subdirectory(lib/ADC)
add_subdirectory(lib/Effects)
pico_set_program_name(Main "Main")
pico_set_program_version(Main "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Main 0)
pico_enable_stdio_usb(Main 1)

# Add the standard library to the build
target_link_libraries(Main
        pico_stdlib
        I2S
        hardware_dma
        hardware_pwm
        hardware_adc
        PWM
        ADC
        Effects
        )

# Add the standard include files to the build
target_include_directories(Main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
      
    ${PICO_SDK_PATH}/src/common/include
    ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include
    ${PICO_SDK_PATH}/src/rp2_common/hardware_pio/include
    ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include
)

pico_add_extra_outputs(Main)

