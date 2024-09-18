cmake_minimum_required(VERSION 3.28)
project(civlike LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")
set(CMAKE_CXX_STANDARD 23)

##### LIBRARY #####

add_library(civlike OBJECT
        civlike/civlike.hh
        civlike/common/color.hh
        civlike/common/geometry.hh
        civlike/rules/fnval.hh
        civlike/rules/ruleset.hh
        civlike/game/direction.hh
        civlike/game/game.hh
        civlike/game/unit.hh
        civlike/game/visualcue.hh
        civlike/game/city.hh
        civlike/collections/id.hh
        civlike/collections/idmap.hh
        civlike/collections/idrefmap.hh
        civlike/collections/countermap.hh
        civlike/command/command.hh
        civlike/command/new_game.cc
        civlike/command/round.cc
        civlike/command/focus.cc
        civlike/command/unit_actions.cc
        civlike/command/city.cc
)

add_library(civlike_shared SHARED $<TARGET_OBJECTS:civlike>)
add_library(civlike_static STATIC $<TARGET_OBJECTS:civlike>)

set_target_properties(civlike PROPERTIES
        VERSION "0.0.1"
        POSITION_INDEPENDENT_CODE 1)
target_include_directories(civlike PUBLIC civlike)

##### EXECUTABLE #####

find_package(PkgConfig)
find_package(SDL2 REQUIRED)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)

find_program(XXD xxd)
if(NOT XXD)
    message(FATAL_ERROR "xxd not found")
endif()

set(FONT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/uiproto/ShareTechMono-Regular.ttf")
set(FONT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/font.h")

add_custom_command(
        OUTPUT ${FONT_HEADER}
        COMMAND ${XXD} -n font -i ${FONT_FILE} > ${FONT_HEADER}
        DEPENDS ${FONT_FILE}
        COMMENT "Generating font header from ${FONT_HEADER}"
)

add_custom_target(generate_font DEPENDS ${FONT_FILE})

add_executable(uiproto
        main.cc
        uiproto/ui.hh
        uiproto/ui.cc
        uiproto/ui_draw.cc
        uiproto/uicity.hh
        uiproto/uicity.cc
        uiproto/text.hh
        uiproto/text.cc
        uiproto/sdl.hh
        ${FONT_HEADER}
)
add_dependencies(uiproto generate_font)

target_include_directories(uiproto PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} civlike)
target_link_directories(uiproto PUBLIC ${SDL2_TTF_LIBRARY_DIRS})
target_link_libraries(uiproto LINK_PUBLIC civlike ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})