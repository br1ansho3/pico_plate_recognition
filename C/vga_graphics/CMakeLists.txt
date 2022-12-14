if (NOT TARGET vga_graphics)

    add_library(vga_graphics INTERFACE)
# pico_generate_pio_header(vga_graphics ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
# pico_generate_pio_header(vga_graphics ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
# pico_generate_pio_header(vga_graphics ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)

    target_sources(vga_graphics INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/vga_graphics.cpp
    )
    target_link_libraries(vga_graphics INTERFACE pico_stdlib hardware_i2c hardware_spi hardware_irq hardware_dma)

    # pico_generate_pio_header(vga_graphics INTERFACE ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
    # pico_generate_pio_header(vga_graphics INTERFACE ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
    # pico_generate_pio_header(vga_graphics INTERFACE ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)
    
endif()