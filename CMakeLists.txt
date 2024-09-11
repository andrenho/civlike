cmake_minimum_required(VERSION 3.28)
project(civlike LANGUAGES CXX)

##### LIBRARY #####

set(MODULES_SRC
        civlike/civlike.ccm
        civlike/common/color.ccm
        civlike/common/geometry.ccm
        civlike/rules/colonization.ccm
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

find_package(SDL2 REQUIRED)

add_executable(uiproto main.cc)

target_sources(uiproto
        PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS uiproto
        FILES uiproto/uiproto.ccm)

target_compile_features(uiproto PUBLIC cxx_std_20)
target_include_directories(uiproto PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(uiproto LINK_PUBLIC civlike ${SDL2_LIBRARIES})