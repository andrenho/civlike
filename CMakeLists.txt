cmake_minimum_required(VERSION 3.28)
project(civlike LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

##### LIBRARY #####

set(MODULES_SRC
        civlike/civlike.ccm
        civlike/common/color.ccm
        civlike/common/geometry.ccm
        civlike/rules/fnval.ccm
        civlike/rules/ruleset.ccm
        civlike/rules/id.ccm
        civlike/game/game.ccm
        civlike/game/unit.ccm
)

add_library(civlike SHARED)

set_target_properties(civlike PROPERTIES VERSION "0.0.1")

target_sources(civlike
    PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS civlike
        FILES ${MODULES_SRC})

target_compile_features(civlike PUBLIC cxx_std_20)

##### EXECUTABLE #####

find_package(PkgConfig)
find_package(SDL2 REQUIRED)
pkg_search_module(SDL2_TTF REQUIRED sdl2_ttf)

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

add_executable(uiproto main.cc ${FONT_HEADER})
add_dependencies(uiproto generate_font)

target_sources(uiproto
        PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS uiproto
        FILES uiproto/uiproto.ccm uiproto/text.ccm)

target_compile_features(uiproto PUBLIC cxx_std_20)
target_include_directories(uiproto PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_link_directories(uiproto PUBLIC ${SDL2_TTF_LIBRARY_DIRS})
target_link_libraries(uiproto LINK_PUBLIC civlike ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})