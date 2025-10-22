 cmake_minimum_required(VERSION 3.11)
project(wavelet-tree LANGUAGES C VERSION 0.1.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED 17)
set(CMAKE_C_STANDARD_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(wavelet-tree src/foo.c)
target_include_directories(wavelet-tree PUBLIC include)

if(MSVC)
    target_compile_options(wavelet-tree PRIVATE /W4 /WX)
else()
    target_compile_options(wavelet-tree PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

