cmake_minimum_required(VERSION 3.5)

idf_component_register(
    SRC_DIRS .
    INCLUDE_DIRS .)

component_compile_options(-DMG_ENABLE_LINES)
component_compile_options(-DMG_ARCH_ESP32)

