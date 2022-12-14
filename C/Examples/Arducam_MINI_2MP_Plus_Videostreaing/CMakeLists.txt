add_executable(Arducam_mini_2mp_plus_videostreaming

        )

# must match with pio filename and executable name from above
pico_generate_pio_header(Arducam_mini_2mp_plus_videostreaming ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
pico_generate_pio_header(Arducam_mini_2mp_plus_videostreaming ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
pico_generate_pio_header(Arducam_mini_2mp_plus_videostreaming ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)


# must match with executable name and source file names
# target_sources(Arducam_mini_2mp_plus_videostreaming PRIVATE Arducam_MINI_2MP_Plus_Videostreaing.cpp)
target_sources(Arducam_mini_2mp_plus_videostreaming PRIVATE Arducam_MINI_2MP_Plus_Videostreaing.cpp layer1.cpp)

# Pull in our pico_stdlib which pulls in commonly used features
# add_subdirectory(../../vga_graphics)
include_directories(../../vga_graphics)
include_directories(../../ArduCAM)

target_link_libraries(Arducam_mini_2mp_plus_videostreaming
	pico_stdlib
	hardware_uart
	hardware_dma
	hardware_i2c
	hardware_pwm
	hardware_pio
	ArduCAM
	pico_multicore
	vga_graphics
	pico_bootsel_via_double_reset
)

pico_enable_stdio_usb(Arducam_mini_2mp_plus_videostreaming 1)
pico_enable_stdio_uart(Arducam_mini_2mp_plus_videostreaming 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(Arducam_mini_2mp_plus_videostreaming)


