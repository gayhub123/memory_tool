cmake_minimum_required(VERSION 3.10)

project(gperftools_test CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "C++ Compiler not found")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)

    # common options for GNUCXX
    add_compile_options(
        $<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>  # https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Code-Gen-Options.html#index-fexceptions
        $<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-frtti>        # https://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/C_002b_002b-Dialect-Options.html#index-fno-rtti
    )

    # common compiling options
    add_compile_options(
        -fno-strict-aliasing
        -fthreadsafe-statics
        -pthread
        -fstack-protector-strong
        -fno-short-enums
        -fPIC
    )

    # warnings
    add_compile_options(
        -Wall               # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wall-263
        -Werror             # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Werror-254
        -Wextra             # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wextra-266
        -Wpedantic          # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wpedantic-261
        -Wconversion        # https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Warning-Options.html#index-Wconversion-426
        -Wempty-body
        -Wfatal-errors
        -Wshadow
        -Wzero-as-null-pointer-constant
        -Wunreachable-code
        -Wmissing-field-initializers
        -Werror=return-type
        -Werror=write-strings
        -Werror=unused-label
        -Werror=switch
    )

    # tcmalloc options
    add_compile_options(
        -fno-builtin-malloc
        -fno-builtin-calloc
        -fno-builtin-realloc
        -fno-builtin-free)

    #==============================================================================
    # Althrough CMake documentation (3.6.3) says add_compile_options supports
    # generator expressions, the actual result is -buggy-.
    # add_compile_options doesn't fully working with generator expressions.
    #
    # $<$<OR:?1,?2>:...>
    # for OR expression, do not insert any whitespace character between ?1 and ?2.
    #==============================================================================
    # options for Debug
    #add_compile_options($<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g2 -ggdb -O0 -fno-omit-frame-pointer>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g3 -ggdb -O0 -fno-omit-frame-pointer>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:DEBUG _DEBUG>)

    # options for Release & RelWithDebInfo
    # add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-fomit-frame-pointer -ffunction-sections -fdata-sections>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-ffunction-sections -fdata-sections>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:NDEBUG>)

    # options for Release
    add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:release>>:-g0>)

    # options for RelWithDebInfo
    add_compile_options($<$<CONFIG:RelWithDebInfo>:-g1>)
    add_compile_options($<$<CONFIG:RelWithDebInfo>:-ggdb>)
else()
    message(FATAL_ERROR "Not supported C++ Compiler: " ${CMAKE_CXX_COMPILER_ID})
endif()

find_package(Threads REQUIRED)

include_directories(src/gperftools/src)
link_directories(src/gperftools/.libs)
link_directories(src/libunwind/src/.libs)
add_subdirectory(tests)
