add_library(lib
    crc.c
    crc.h
    demo_logging.c
    diskio.c
    diskio.h
    f_util.c
    f_util.h
    ff_stdio.c
    ff_stdio.h
    ff.c
    ff.h
    ffconf.h
    ffsystem.c
    ffunicode.c
    glue.c
    hw_config.c
    hw_config.h
    my_debug.c
    my_debug.h
    rtc.c
    rtc.h
    sd_card.c
    sd_card.h
    sd_spi.c
    sd_spi.h
    spi.c
    spi.h
    util.h
    data_logger.cpp
    data_logger.h
)

target_link_libraries(lib
    hardware_clocks
    hardware_resets
    hardware_spi
    hardware_dma
    hardware_rtc
    pico_stdlib

)

target_include_directories(lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")