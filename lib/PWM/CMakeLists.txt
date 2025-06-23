add_library(PWM PWM.c)
pico_generate_pio_header(PWM ${CMAKE_CURRENT_LIST_DIR}/PWM.pio)
target_link_libraries(PWM
    pico_stdlib
    hardware_pio
    hardware_pwm
    hardware_dma
)
target_include_directories(PWM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})