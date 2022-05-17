idf_component_register(
    SRC_DIRS . telnet modules modules/http modules/lvgl modules/lvgl/driver modules/lvgl/font libb64 gameplayer/nofrendo/cpu gameplayer/nofrendo/libsnss gameplayer/nofrendo/nes gameplayer/nofrendo/sndhrdw gameplayer/nofrendo/mappers gameplayer/nofrendo gameplayer media
    INCLUDE_DIRS . telnet gameplayer modules modules/http modules/lvgl modules/lvgl/driver modules/lvgl/font libb64 gameplayer/nofrendo/cpu gameplayer/nofrendo/libsnss gameplayer/nofrendo/nes gameplayer/nofrendo/sndhrdw gameplayer/nofrendo gameplayer media
)


# for nofrendo    
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=char-subscripts -Wno-error=attributes")
