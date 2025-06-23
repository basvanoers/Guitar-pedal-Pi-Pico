add_library(Effects Effects.c)

target_link_libraries(Effects
    pico_stdlib
    hardware_pio
    hardware_adc
    hardware_dma
    hardware_pwm
    PWM
)
target_include_directories(Effects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})