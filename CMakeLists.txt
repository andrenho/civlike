cmake_minimum_required(VERSION 3.28)
project(civlike LANGUAGES CXX)

set(MODULES_SRC hello/hello.ccm)

add_executable(civlike)
target_sources(civlike
    PUBLIC main.cc
    PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS hello
        FILES ${MODULES_SRC})

target_compile_features(civlike PUBLIC cxx_std_20)
