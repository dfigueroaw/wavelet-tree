cmake_minimum_required(VERSION 3.11)
project(wavelet-tree LANGUAGES C VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED 99)
set(CMAKE_C_STANDARD_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GCC_LIKE $<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>)

add_library(wavelet-tree src/wavelet.c)
target_include_directories(wavelet-tree PUBLIC include)

target_compile_options(wavelet-tree PUBLIC
    $<$<BOOL:${GCC_LIKE}>:-Wall>
    $<$<BOOL:${GCC_LIKE}>:-Wextra>
    $<$<BOOL:${GCC_LIKE}>:-Wpedantic>
    $<$<BOOL:${GCC_LIKE}>:-Wconversion>
    $<$<BOOL:${GCC_LIKE}>:-O0>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>)

add_executable(wavelet-benchmark src/benchmark.c)
target_link_libraries(wavelet-benchmark PRIVATE wavelet-tree)

add_executable(wavelet-entropy src/entropy.c)
target_link_libraries(wavelet-entropy PRIVATE wavelet-tree)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

