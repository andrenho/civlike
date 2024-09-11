cmake_minimum_required(VERSION 3.28)
project(civlike LANGUAGES CXX)

set(MODULES_SRC
        common/color.ccm
        common/geometry.ccm
        ruleset/colonization.ccm
        ruleset/ruleset.ccm
        ruleset/id.ccm
)

add_executable(civlike
        main.cc)

target_sources(civlike
    PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS ruleset common
        FILES ${MODULES_SRC})

target_compile_features(civlike PUBLIC cxx_std_20)