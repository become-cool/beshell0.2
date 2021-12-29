cmake_minimum_required(VERSION 3.10)

project(beshell)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")


LINK_LIBRARIES(m)
LINK_LIBRARIES(pthread)

add_definitions(-g)
add_definitions(-DSIMULATION)
# add_definitions(-DWIN32)
# add_definitions(-DMG_ENABLE_LINES)
add_definitions(-DLV_THEME_DEFAULT_DARK)
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)


SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_C_FLAGS_DEBUG "-O0")


# ssl
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)
add_definitions(-lssl -lcrypto -ldl)


# lvgl
# add_definitions(-DLV_CONF_SKIP)
include_directories(../components/lvgl/src)
file(GLOB_RECURSE SRC_LIST "../components/lvgl/src/*.c")

# quickjs
AUX_SOURCE_DIRECTORY(../components/quickjs SRC_LIST)
include_directories(../components/quickjs)

# mongoose
AUX_SOURCE_DIRECTORY(../components/mongoose SRC_LIST)
include_directories(../components/mongoose)


AUX_SOURCE_DIRECTORY(. SRC_LIST)

include_directories(./)
include_directories(../main)
include_directories(../main/anchor9)
include_directories(../main/modules)
include_directories(../main/modules/lvgl)
include_directories(../main/modules/lvgl/font)

list(APPEND SRC_LIST ../main/utils.c)
list(APPEND SRC_LIST ../main/eventloop.c)
list(APPEND SRC_LIST ../main/task_js.c)
list(APPEND SRC_LIST ../main/mallocf.c)
list(APPEND SRC_LIST ../main/stack.c)
list(APPEND SRC_LIST ../main/list.c)
list(APPEND SRC_LIST ../main/uuid.c)
list(APPEND SRC_LIST ../main/font/s16.c)
list(APPEND SRC_LIST ../main/anchor9/anchor9.c)
list(APPEND SRC_LIST ../main/anchor9/anchor9_lvgl.c)
list(APPEND SRC_LIST ../main/modules/module_fs.c)
list(APPEND SRC_LIST ../main/modules/module_utils.c)
list(APPEND SRC_LIST ../main/modules/module_mg.c)
list(APPEND SRC_LIST ../main/modules/module_process.c)
list(APPEND SRC_LIST ../main/modules/lvgl/module_lvgl.c)
list(APPEND SRC_LIST ../main/modules/lvgl/display.c)
list(APPEND SRC_LIST ../main/modules/lvgl/style.c)
list(APPEND SRC_LIST ../main/modules/lvgl/be_lv_struct_wrapper.c)
list(APPEND SRC_LIST ../main/modules/lvgl/widgets.c)
list(APPEND SRC_LIST ../main/modules/lvgl/widgets_gen.c)
list(APPEND SRC_LIST ../main/modules/lvgl/font/font_msyh.c)

ADD_EXECUTABLE(beshell ${SRC_LIST})
target_link_libraries(beshell libssl.a libcrypto.a dl)
target_link_libraries(beshell dl pthread)

set_property(TARGET beshell PROPERTY C_STANDARD 99)