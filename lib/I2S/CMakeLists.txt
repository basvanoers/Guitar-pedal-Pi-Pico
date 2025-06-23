add_library(I2S I2S.c)
pico_generate_pio_header(I2S ${CMAKE_CURRENT_LIST_DIR}/I2S.pio)
target_link_libraries(I2S
    pico_stdlib
    hardware_pio
)
target_include_directories(I2S PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})