cmake_minimum_required(VERSION 3.28)
project(civlike LANGUAGES CXX)

set(MODULES_SRC
        civlike/civlike.ccm
        civlike/common/color.ccm
        civlike/common/geometry.ccm
        civlike/rules/colonization.ccm
        civlike/rules/ruleset.ccm
        civlike/rules/id.ccm
)

add_executable(civlike
        main.cc)

target_sources(civlike
    PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS civlike
        FILES ${MODULES_SRC})

target_compile_features(civlike PUBLIC cxx_std_20)