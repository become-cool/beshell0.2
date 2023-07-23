cmake_minimum_required(VERSION 3.10)
set(idf_version "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}.${IDF_VERSION_PATCH}")

if (NOT DEFINED PROJECT_NAME)
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(esp_littlefs)
else ()
    file(GLOB SOURCES src/littlefs/*.c)
    list(APPEND SOURCES src/esp_littlefs.c src/littlefs_api.c)
    
    # < 5.0
    if ("${idf_version}" VERSION_LESS "5.0")
        idf_component_register(s
                SRCS ${SOURCES}
                INCLUDE_DIRS src include
                REQUIRES spi_flash)
    else()
        idf_component_register(
            SRCS ${SOURCES}
            INCLUDE_DIRS src include
            REQUIRES spi_flash esp_partition vfs)
    endif()

endif (NOT DEFINED PROJECT_NAME)

component_compile_options(-Wno-error=format= -Wno-format)
