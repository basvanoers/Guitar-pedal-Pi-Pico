add_library(ADC ADC.c)

target_link_libraries(ADC
    pico_stdlib
    hardware_pio
    hardware_adc
    hardware_dma
)
target_include_directories(ADC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})