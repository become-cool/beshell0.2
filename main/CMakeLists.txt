idf_component_register(
    SRC_DIRS . telnet modules modules/http modules/mg modules/driver modules/lvgl modules/lvgl/driver modules/lvgl/font libb64 gameplayer/nofrendo/cpu gameplayer/nofrendo/libsnss gameplayer/nofrendo/nes gameplayer/nofrendo/sndhrdw gameplayer/nofrendo/mappers gameplayer/nofrendo gameplayer media lv_demo_music lv_demo_music/assets
    INCLUDE_DIRS . telnet gameplayer modules modules/http modules/mg modules/driver modules/lvgl modules/lvgl/driver modules/lvgl/font libb64 gameplayer/nofrendo/cpu gameplayer/nofrendo/libsnss gameplayer/nofrendo/nes gameplayer/nofrendo/sndhrdw gameplayer/nofrendo gameplayer media lv_demo_music
    # EMBED_FILES root.raw
)

# for mongoose tls/ssl
add_definitions(-DMG_ENABLE_OPENSSL)

# for nofrendo
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-error=char-subscripts -Wno-error=attributes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=char-subscripts -Wno-error=attributes")

